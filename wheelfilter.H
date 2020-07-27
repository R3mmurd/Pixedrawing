/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# ifndef WHEELFILTER_H
# define WHEELFILTER_H

# include <QObject>

class DrawingPanelWrapper;

class WheelFilter : public QObject
{
  Q_OBJECT

  QObject * obj;

  DrawingPanelWrapper * panel;

public:
  WheelFilter(QObject *, DrawingPanelWrapper *);

  bool eventFilter(QObject *, QEvent *) override;
};

# endif // WHEELFILTER_H
