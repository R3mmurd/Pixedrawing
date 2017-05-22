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
# ifndef DRAWINGPANEL_H
# define DRAWINGPANEL_H

# include <QWidget>
# include <QList>

# include <drawinglattice.H>

class DrawingPanel : public QWidget
{
  Q_OBJECT

public:
  static constexpr size_t SCALE = 8;

private:
  DrawingLattice lattice;

  double zoom_factor = 1.0;

  DrawingLattice::LayerSet::size_type current_layer = 0;

  QColor color_to_paint = Qt::black;

  Qt::MouseButton pressed_button;

  QList<std::tuple<QColor, size_t, size_t, size_t>> painted_cells;

  void adjust_size();

protected:
  void paintEvent(QPaintEvent *) override;

  void mousePressEvent(QMouseEvent *) override;

  void mouseMoveEvent(QMouseEvent *) override;

  void mouseReleaseEvent(QMouseEvent *) override;

public:
  DrawingPanel(size_t, size_t, QWidget * parent = nullptr);

  const QColor & get_color_to_paint() const;

  void set_color_to_paint(const QColor &);

  void zoom_in();

  void zoom_out();

  void zoom_1();

  const double & get_zoom_factor() const;

  void save_to_file(QString &);

  void load_from_file(QString &);

  QImage export_bitmap();

  const size_t & get_rows() const;

  const size_t & get_cols() const;

  void redim(size_t, size_t);

  DrawingLattice & get_lattice();

  void set_current_layer(DrawingLattice::LayerSet::size_type);

  const DrawingLattice::LayerSet::size_type & get_current_layer() const;

  std::tuple<QString, bool>
  get_layer_info(DrawingLattice::LayerSet::size_type) const;

  QList<std::tuple<QString, bool>> get_layer_info() const;

signals:
  void signal_changed();

  void signal_painted(QList<std::tuple<QColor, size_t, size_t, size_t>>);

public slots:
  void slot_change_visibility(bool);
};

# endif // DRAWINGPANEL_H
