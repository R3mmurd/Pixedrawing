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
# include <drawingpanel.H>

# include <QPainter>
# include <QMouseEvent>
# include <QFile>

void DrawingPanel::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(0, 0, width(), height(), Qt::white);

  double w = SCALE * zoom_factor;
  double h = SCALE * zoom_factor;

  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      painter.fillRect(j * w, i * h, w, h, lattice[i][j]);

  painter.setPen(Qt::darkGray);

  for (size_t i = 0; i < rows; ++i)
    painter.drawLine(0, i * w, width(), i * h);

  for (size_t j = 0; j < cols; ++j)
    painter.drawLine(j * w, 0, j * h, height());
}

void DrawingPanel::mousePressEvent(QMouseEvent * evt)
{
  pressed_button = evt->button();

  if (pressed_button != Qt::LeftButton and pressed_button != Qt::RightButton)
    return;

  const QPoint & pos = evt->pos();

  size_t i = pos.y() / (SCALE * zoom_factor);
  size_t j = pos.x() / (SCALE * zoom_factor);

  if (i >= rows or j >= cols)
    return;

  if (pressed_button == Qt::LeftButton)
    lattice[i][j] = color_to_paint;
  else
    lattice[i][j] = Qt::transparent;

  emit signal_changed();

  repaint();
}

void DrawingPanel::mouseMoveEvent(QMouseEvent * evt)
{
  if (pressed_button != Qt::LeftButton and pressed_button != Qt::RightButton)
    return;

  const QPoint & pos = evt->pos();

  size_t i = pos.y() / (SCALE * zoom_factor);
  size_t j = pos.x() / (SCALE * zoom_factor);

  if (i >= rows or j >= cols)
    return;

  if (pressed_button == Qt::LeftButton)
    lattice[i][j] = color_to_paint;
  else
    lattice[i][j] = Qt::transparent;

  emit signal_changed();

  repaint();
}

DrawingPanel::DrawingPanel(QWidget * parent)
  : QWidget(parent)
{
  lattice = allocate_lattice(rows, cols);
  resize(cols * SCALE * zoom_factor, rows * SCALE * zoom_factor);
}

DrawingPanel::~DrawingPanel()
{
  free_lattice(lattice, rows);
}

void DrawingPanel::redim(size_t r, size_t c)
{
  QColor ** tmp = allocate_lattice(r, c);

  size_t _r = std::min(r, rows);
  size_t _c = std::min(c, cols);

  for (size_t i = 0; i < _r; ++i)
    for (size_t j = 0; j < _c; ++j)
      tmp[i][j] = lattice[i][j];

  free_lattice(lattice, rows);
  rows = r;
  cols = c;
  lattice = tmp;
  resize(cols * SCALE * zoom_factor, rows * SCALE * zoom_factor);
}

const size_t & DrawingPanel::get_rows() const
{
  return rows;
}

const size_t & DrawingPanel::get_cols() const
{
  return cols;
}

const QColor & DrawingPanel::get_color_to_paint() const
{
  return color_to_paint;
}

void DrawingPanel::set_color_to_paint(const QColor & c)
{
  color_to_paint = c;
}

void DrawingPanel::save_to_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::WriteOnly))
    throw std::runtime_error("Cannot create file");

  file.write(reinterpret_cast<char *>(&rows), sizeof(size_t));
  file.write(reinterpret_cast<char *>(&cols), sizeof(size_t));

  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      {
        int c = lattice[i][j].red();
        file.write(reinterpret_cast<char *>(&c), sizeof(int));
        c = lattice[i][j].green();
        file.write(reinterpret_cast<char *>(&c), sizeof(int));
        c = lattice[i][j].blue();
        file.write(reinterpret_cast<char *>(&c), sizeof(int));
        c = lattice[i][j].alpha();
        file.write(reinterpret_cast<char *>(&c), sizeof(int));
      }

  file.close();
}

void DrawingPanel::load_from_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::ReadOnly))
    throw std::runtime_error("File doesn't exist");

  size_t r, c;

  file.read(reinterpret_cast<char *>(&r), sizeof(size_t));
  file.read(reinterpret_cast<char *>(&c), sizeof(size_t));

  redim(r, c);

  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      {
        int c;
        file.read(reinterpret_cast<char *>(&c), sizeof(int));
        lattice[i][j].setRed(c);
        file.read(reinterpret_cast<char *>(&c), sizeof(int));
        lattice[i][j].setGreen(c);
        file.read(reinterpret_cast<char *>(&c), sizeof(int));
        lattice[i][j].setBlue(c);
        file.read(reinterpret_cast<char *>(&c), sizeof(int));
        lattice[i][j].setAlpha(c);
      }

  file.close();
}

QImage DrawingPanel::export_bitmap()
{
  QImage ret_val(cols, rows, QImage::Format_ARGB32);

  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      ret_val.setPixelColor(j, i, lattice[i][j]);

  return ret_val;
}

void DrawingPanel::zoom_in()
{
  zoom_factor += 0.1;
  resize(cols * SCALE * zoom_factor, rows * SCALE * zoom_factor);
  repaint();
}

void DrawingPanel::zoom_out()
{
  zoom_factor -= 0.1;
  resize(cols * SCALE * zoom_factor, rows * SCALE * zoom_factor);
  repaint();
}

void DrawingPanel::zoom_1()
{
  zoom_factor = 1.0;
  resize(cols * SCALE * zoom_factor, rows * SCALE * zoom_factor);
  repaint();
}

const double & DrawingPanel::get_zoom_factor() const
{
  return zoom_factor;
}
