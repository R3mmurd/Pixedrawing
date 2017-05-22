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
# ifndef DOCKLAYERS_H
# define DOCKLAYERS_H

# include <QTableWidget>
# include <QDockWidget>
# include <QPushButton>
# include <QList>

class CheckWidget : public QWidget
{
  Q_OBJECT

  bool checked;

protected:
  void mousePressEvent(QMouseEvent *) override;

  void paintEvent(QPaintEvent *) override;

public:
  CheckWidget(bool, QWidget * parent = nullptr);

  bool is_checked() const;

signals:
  void signal_change_visibility(bool, QPoint);
};

class DockLayers : public QDockWidget
{
  Q_OBJECT

  QTableWidget * table;

  int selected_row = -1;

  void select_row(int);

protected:
  void closeEvent(QCloseEvent *) override;

public:
  DockLayers(QWidget * parent = nullptr);

  void add_layer_info(const QList<std::tuple<QString, bool>> &);

  void add_layer_info(const std::tuple<QString, bool> &);

  void remove_layer_info(int);

  void clear();

public slots:
  void slot_item_changed();

  void slot_change_visibility(bool, QPoint);

  void slot_change_name(int, int);

signals:
  void signal_close();

  void signal_change_visibility(bool, int);

  void signal_change_name(QString, int);

  void signal_change_selected_layer(int);
};

# endif // DOCKLAYERS_H
