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
# ifndef CUSTOMREDIMDIALOG_H
# define CUSTOMREDIMDIALOG_H

# include <QDialog>
# include <ui_customredimdialog.h>

class CustomRedimDialog : public QDialog
{
  Q_OBJECT

public:
  Ui_CustomRedimDialog ui;

  CustomRedimDialog(QWidget * parent = nullptr);
};

# endif // CUSTOMREDIMDIALOG_H