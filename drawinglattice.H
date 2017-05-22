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
# ifndef DRAWINGLATTICE_H
# define DRAWINGLATTICE_H

# include <QColor>
# include <QVector>
# include <QImage>

# include <helpers.H>

class DrawingLattice : public QObject
{
  Q_OBJECT

public:
  struct Layer
  {
    QColor ** mat = nullptr;
    QString name = "";
    bool visible = true;

    Layer();

    Layer(QColor **, const QString &);
  };

  using LayerSet = QVector<Layer>;

private:
  size_t rows;
  size_t cols;

  LayerSet layers;

public:
  DrawingLattice(size_t, size_t);

  ~DrawingLattice();

  void clear();

  void redim(size_t, size_t);

  const size_t & get_rows() const;

  const size_t & get_cols() const;

  LayerSet::size_type get_num_layers() const;

  void save_to_file(QString &);

  void load_from_file(QString &);

  QImage export_bitmap();

  void paint(size_t, size_t, size_t, const QColor &);

  const QColor & get_color(size_t, size_t, size_t) const;

  void draw(QPainter &, double, double);

  void show_layer(LayerSet::size_type);

  void hide_layer(LayerSet::size_type);

  bool is_layer_visible(LayerSet::size_type) const;

  const QString & get_layer_name(LayerSet::size_type) const;

  void set_layer_name(LayerSet::size_type, const QString &);

  void add_layer_front();

  void add_layer_back();

  void remove_layer(LayerSet::size_type);

signals:
  void signal_change_visibility(bool);

  void signal_layer_added(QString);
};

# endif // DRAWINGLATTICE_H
