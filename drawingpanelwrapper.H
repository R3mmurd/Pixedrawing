/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
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
