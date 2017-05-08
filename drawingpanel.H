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
# include <QColor>
# include <QImage>

class DrawingPanel : public QWidget
{
  Q_OBJECT

public:
  static constexpr size_t SCALE = 8;

private:
  size_t rows = 64;
  size_t cols = 64;

  double zoom_factor = 1.0;

  QColor ** lattice = nullptr;

  QColor color_to_paint = Qt::black;

  Qt::MouseButton pressed_button;

  void allocate_lattice();

  void free_lattice();

  void redim();

protected:
  void paintEvent(QPaintEvent *) override;

  void mousePressEvent(QMouseEvent *) override;

  void mouseMoveEvent(QMouseEvent *) override;

public:
  DrawingPanel(QWidget * parent = nullptr);

  ~DrawingPanel();

  void redim(size_t, size_t);

  const size_t & get_rows() const;

  const size_t & get_cols() const;

  void set_color_to_paint(const QColor &);

  void save_to_file(QString &);

  void load_from_file(QString &);

  QImage to_image();

  void zoom_in();

  void zoom_out();

  void zoom_1();

  const double & get_zoom_factor() const;

signals:
  void signal_changed();
};

# endif // DRAWINGPANEL_H
