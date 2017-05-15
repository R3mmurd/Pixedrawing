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
# ifndef DRAWINGPANELWRAPPER_H
# define DRAWINGPANELWRAPPER_H

# include <QScrollArea>

# include <drawingpanel.H>
# include <wheelfilter.H>

class DrawingPanelWrapper : public QScrollArea
{
  Q_OBJECT

public:
  static constexpr double MAX_ZOOM_FACTOR = 4.0;
  static constexpr double MIN_ZOOM_FACTOR = 0.4;

private:
  DrawingPanel * drawing_panel;
  WheelFilter * viewport_filter;
  WheelFilter * horizontal_bar_filter;
  WheelFilter * vertical_bar_filter;

  void init_gui();

  void configure_filters();

  void zoom();

public:
  DrawingPanelWrapper(QWidget * parent = nullptr);

  void wheelEvent(QWheelEvent *) override;

  void closeEvent(QCloseEvent *) override;

  DrawingPanel * get_drawing_panel();

signals:
  void signal_zoom(double);

public slots:
  void slot_zoom_in();

  void slot_zoom_out();

  void slot_zoom_1();
};

# endif // DRAWINGPANELWRAPPER_H
