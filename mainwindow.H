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
# ifndef MAINWINDOW_H
# define MAINWINDOW_H

# include <QMainWindow>
# include <QMenu>
# include <QToolBar>
# include <QAction>
# include <QVector>

# include <drawingpanelwrapper.H>
# include <customredimdialog.H>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  static const     char * EXT;
  static constexpr size_t NUM_DFT_REDIM_VALUES = 6;
  static constexpr size_t NUM_RECENT_COLORS    = 5;
  static constexpr int    STATUS_BAR_TIME      = 3000;

  QString last_visited_path;

  bool is_saved = true;

  QString name = "";

  QVector<QColor> recent_colors;

  DrawingPanelWrapper * drawing_panel_wrapper;
  DrawingPanel * drawing_panel;

  QAction * action_new;
  QAction * action_save;
  QAction * action_save_as;
  QAction * action_open;
  QAction * action_export;
  QAction * action_exit;

  QAction * actions_redim[NUM_DFT_REDIM_VALUES + 1];
  QAction * actions_pick_color[NUM_RECENT_COLORS + 1];

  QAction * action_zoom_in;
  QAction * action_zoom_out;
  QAction * action_zoom_1;

  QAction * action_about;
  QAction * action_about_qt;

  QMenu * menu_file;
  QMenu * menu_edit;
  QMenu * menu_color_list;
  QMenu * menu_redim;
  QMenu * menu_view;
  QMenu * menu_help;

  QToolBar * tool_bar;

  void init_actions();

  void init_menu();

  void init_toolbar();

  void init_gui();

  void save_file();

  void set_title();

  void create_work();

  void close_work();

public:
  MainWindow(QWidget * parent = nullptr);

  void closeEvent(QCloseEvent *) override;

public slots:
  void slot_changed();

  void slot_new();

  void slot_save();

  void slot_save_as();

  void slot_open();

  void slot_export();

  void slot_set_color(QColor);

  void slot_set_recent_color();

  void slot_pick_color();

  void slot_redim();

  void slot_custom_redim();

  void slot_update_zoom(double);

  void slot_about();

  void slot_about_qt();
};

# endif // MAINWINDOW_H