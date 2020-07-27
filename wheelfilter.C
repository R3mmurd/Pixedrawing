/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <QWheelEvent>
# include <wheelfilter.H>
# include <drawingpanelwrapper.H>

WheelFilter::WheelFilter(QObject * o, DrawingPanelWrapper * p)
    : obj(o), panel(p)
{
  // Empty
}

bool WheelFilter::eventFilter(QObject * _obj, QEvent * evt)
{
  if (_obj == obj and evt->type() == QEvent::Wheel)
    {
      QWheelEvent * wevt = static_cast <QWheelEvent *>(evt);

      if (wevt->modifiers().testFlag(Qt::ControlModifier))
        {
          panel->wheelEvent(wevt);
          return true;
        }
    }
  return false;
}
