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
# include <drawingpanelwrapper.H>

# include <QWheelEvent>
# include <QScrollBar>

void DrawingPanelWrapper::init_gui()
{
  drawing_panel = new DrawingPanel(this);

  scroll_area = new QScrollArea(this);
  scroll_area->setBackgroundRole(QPalette::Dark);
  scroll_area->setAlignment(Qt::AlignCenter);
  scroll_area->setWidget(drawing_panel);

  layout = new QVBoxLayout(this);
  layout->addWidget(scroll_area);
  setLayout(layout);
}

void DrawingPanelWrapper::configure_filters()
{
  viewport_filter = new WheelFilter(scroll_area->viewport(), this);
  horizontal_bar_filter = new WheelFilter(scroll_area->horizontalScrollBar(),
                                          this);
  vertical_bar_filter = new WheelFilter(scroll_area->verticalScrollBar(), this);

  scroll_area->viewport()->installEventFilter(viewport_filter);
  scroll_area->horizontalScrollBar()->
      installEventFilter(horizontal_bar_filter);
  scroll_area->verticalScrollBar()->installEventFilter(vertical_bar_filter);

  installEventFilter(this);
}

void DrawingPanelWrapper::adjust_scroll_bar(QScrollBar * scroll_bar,
                                            double factor)
{
  scroll_bar->setValue(int(factor * scroll_bar->value() +
                           ((factor - 1) * scroll_bar->pageStep() / 2)));
}

void DrawingPanelWrapper::zoom()
{
  double factor = drawing_panel->get_zoom_factor();
  emit signal_zoom(factor);
  adjust_scroll_bar(scroll_area->horizontalScrollBar(), factor);
  adjust_scroll_bar(scroll_area->verticalScrollBar(), factor);
}

DrawingPanelWrapper::DrawingPanelWrapper(QWidget * parent)
  : QWidget(parent)
{
  init_gui();
  configure_filters();
}

void DrawingPanelWrapper::wheelEvent(QWheelEvent * evt)
{
  if (not evt->modifiers().testFlag(Qt::ControlModifier))
    return;

  double factor = drawing_panel->get_zoom_factor();

  if (evt->delta() > 0 and factor <= MAX_ZOOM_FACTOR)
    slot_zoom_in();
  else if (factor >= MIN_ZOOM_FACTOR)
    slot_zoom_out();
}

void DrawingPanelWrapper::closeEvent(QCloseEvent *)
{
  delete viewport_filter;
  delete horizontal_bar_filter;
  delete vertical_bar_filter;
}

DrawingPanel *DrawingPanelWrapper::get_drawing_panel()
{
  return drawing_panel;
}

void DrawingPanelWrapper::slot_zoom_in()
{
  drawing_panel->zoom_in();
  zoom();
}

void DrawingPanelWrapper::slot_zoom_out()
{
  drawing_panel->zoom_out();
  zoom();
}

void DrawingPanelWrapper::slot_zoom_1()
{
  drawing_panel->zoom_1();
  zoom();
}
