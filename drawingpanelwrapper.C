/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <drawingpanelwrapper.H>

# include <QWheelEvent>
# include <QScrollBar>

void DrawingPanelWrapper::init_gui()
{
  drawing_panel = new DrawingPanel(DftValues::DRAWING_PANEL_SIZE,
                                   DftValues::DRAWING_PANEL_SIZE,this);

  viewport()->setBackgroundRole(QPalette::Dark);
  viewport()->setAutoFillBackground(true);
  setAlignment(Qt::AlignCenter);
  setWidget(drawing_panel);
}

void DrawingPanelWrapper::configure_filters()
{
  viewport_filter = new WheelFilter(viewport(), this);
  horizontal_bar_filter = new WheelFilter(horizontalScrollBar(),
                                          this);
  vertical_bar_filter = new WheelFilter(verticalScrollBar(), this);

  viewport()->installEventFilter(viewport_filter);
  horizontalScrollBar()->
      installEventFilter(horizontal_bar_filter);
  verticalScrollBar()->installEventFilter(vertical_bar_filter);

  installEventFilter(this);
}

void DrawingPanelWrapper::zoom()
{
  double factor = drawing_panel->get_zoom_factor();
  emit signal_zoom(factor);
}

DrawingPanelWrapper::DrawingPanelWrapper(QWidget * parent)
  : QScrollArea(parent)
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
