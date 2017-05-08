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
