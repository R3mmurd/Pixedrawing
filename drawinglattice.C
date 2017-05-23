/*
  This file is part of Pixedrawing.
  Copyright (C) 2017 by Alejandro J. Mujica

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Any user request of this software, write to

  Alejandro Mujica

  aledrums@gmail.com
*/
# include <drawinglattice.H>

# include <QPainter>
# include <QFile>

DrawingLattice::DrawingLattice(size_t r, size_t c)
  : rows(r), cols(c)
{
  layers.push_back(Layer(allocate_lattice(rows, cols, Qt::white),
                         "Background"));
}

DrawingLattice::~DrawingLattice()
{
  clear();
}

void DrawingLattice::clear()
{
  for (Layer & l : layers)
    free_lattice(l.mat, rows);

  layers.clear();
}

void DrawingLattice::redim(size_t r, size_t c)
{
  size_t _r = std::min(r, rows);
  size_t _c = std::min(c, cols);

  for (Layer & l : layers)
    {
      QColor ** tmp = allocate_lattice(r, c);

      for (size_t i = 0; i < _r; ++i)
        for (size_t j = 0; j < _c; ++j)
          tmp[i][j] = l.mat[i][j];

      free_lattice(l.mat, rows);
      l.mat = tmp;
    }

  rows = r;
  cols = c;
}

const size_t & DrawingLattice::get_rows() const
{
  return rows;
}

const size_t & DrawingLattice::get_cols() const
{
  return cols;
}

DrawingLattice::LayerSet::size_type DrawingLattice::get_num_layers() const
{
  return layers.size();
}

void DrawingLattice::save_to_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::WriteOnly))
    throw std::runtime_error("Cannot create file");

  LayerSet::size_type num_layers = layers.size();

  file.write(reinterpret_cast<char *>(&num_layers),sizeof(LayerSet::size_type));
  file.write(reinterpret_cast<char *>(&rows), sizeof(size_t));
  file.write(reinterpret_cast<char *>(&cols), sizeof(size_t));

  for (Layer & l : layers)
    {
      QString::size_type sz = l.name.size() + 1;
      file.write(reinterpret_cast<char *>(&sz), sizeof(QString::size_type));
      file.write(l.name.toStdString().c_str(), sz);
      file.write(reinterpret_cast<char *>(&l.visible), sizeof(bool));
      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
        {
          int c = l.mat[i][j].red();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].green();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].blue();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].alpha();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
        }
    }

  file.close();
}

void DrawingLattice::load_from_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::ReadOnly))
    throw std::runtime_error("File doesn't exist");

  clear();

  LayerSet::size_type num_layers;

  file.read(reinterpret_cast<char *>(&num_layers), sizeof(LayerSet::size_type));
  file.read(reinterpret_cast<char *>(&rows), sizeof(size_t));
  file.read(reinterpret_cast<char *>(&cols), sizeof(size_t));

  for (LayerSet::size_type l = 0; l < num_layers; ++l)
    {
      Layer layer;

      QString::size_type sz;
      file.read(reinterpret_cast<char *>(&sz), sizeof(QString::size_type));
      char str[sz];
      file.read(str, sz);
      layer.name = str;
      file.read(reinterpret_cast<char *>(&layer.visible), sizeof(bool));

      layer.mat = allocate_lattice(rows, cols);

      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
          {
            int c;
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setRed(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setGreen(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setBlue(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setAlpha(c);
          }

        layers.push_back(layer);
      }

  file.close();
}

QImage DrawingLattice::export_bitmap()
{
  QImage ret_val(cols, rows, QImage::Format_ARGB32);

  for (LayerSet::size_type l = layers.size(); l > 0; --l)
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j)
        ret_val.setPixelColor(j, i, layers[l-1].mat[i][j]);

  return ret_val;
}

void DrawingLattice::paint(size_t l, size_t i, size_t j, const QColor & c)
{
  layers[l].mat[i][j] = c;
}

const QColor & DrawingLattice::get_color(size_t l, size_t i, size_t j) const
{
  return layers[l].mat[i][j];
}

void DrawingLattice::draw(QPainter & painter, double w, double h)
{
  for (LayerSet::size_type l = layers.size(); l > 0; --l)
    {
      if (not layers[l-1].visible)
        continue;

      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
          painter.fillRect(j * w, i * h, w, h, layers[l-1].mat[i][j]);
    }
}

void DrawingLattice::show_layer(LayerSet::size_type l)
{
  if (layers[l].visible)
    return;

  layers[l].visible = true;
  emit signal_change_visibility(true);
}

void DrawingLattice::hide_layer(LayerSet::size_type l)
{
  if (not layers[l].visible)
    return;

  layers[l].visible = false;
  emit signal_change_visibility(false);
}

bool DrawingLattice::is_layer_visible(LayerSet::size_type l) const
{
  return layers[l].visible;
}

const QString &
DrawingLattice::get_layer_name(DrawingLattice::LayerSet::size_type l) const
{
  return layers[l].name;
}

void DrawingLattice::set_layer_name(DrawingLattice::LayerSet::size_type l,
                                    const QString & n)
{
  layers[l].name = n;
}

void DrawingLattice::add_layer_front()
{
  QString name = "layer_";
  name.append(QString::number(layers.size()));
  layers.push_front(Layer(allocate_lattice(rows, cols), name));
  emit signal_layer_added(name);
}

void DrawingLattice::add_layer_back()
{
  QString name = "layer_";
  name.append(QString::number(layers.size()));
  layers.push_back(Layer(allocate_lattice(rows, cols), name));
  emit signal_layer_added(name);
}

void DrawingLattice::remove_layer(DrawingLattice::LayerSet::size_type l)
{
  for ( ; l < layers.size() - 1; ++l)
    std::swap(layers[l], layers[l + 1]);

  free_lattice(layers.back().mat, rows);
  layers.pop_back();
}

DrawingLattice::Layer::Layer()
{
  // empty, dummy constuctor
}

DrawingLattice::Layer::Layer(QColor ** l, const QString & n)
  : mat(l), name(n)
{
  // Empty
}
