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

# include <undocommands.H>
# include <drawingpanelwrapper.H>
# include <customredimdialog.H>
# include <docklayers.H>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  QString last_visited_path;

  bool saved = true;

  QString name = "";

  QVector<QColor> recent_colors;

  DrawingPanelWrapper * drawing_panel_wrapper;
  DrawingPanel * drawing_panel;
  DockLayers * dock_layers;

  QUndoStack undo_stack;

  QLabel * position_label;

  QAction * action_new;
  QAction * action_save;
  QAction * action_save_as;
  QAction * action_open;
  QAction * action_export;
  QAction * action_exit;

  QAction * action_undo;
  QAction * action_redo;
  QAction * actions_redim[DftValues::NUM_DFT_REDIM_VALUES + 1];
  QAction * actions_pick_color[DftValues::NUM_RECENT_COLORS + 1];
  QAction * action_change_background_color;

  QAction * action_show_dock_layers;
  QAction * action_zoom_in;
  QAction * action_zoom_out;
  QAction * action_zoom_1;

  QAction * action_new_layer;
  QAction * action_remove_layer;

  QAction * action_about;
  QAction * action_about_qt;

  QMenu * menu_file;
  QMenu * menu_edit;
  QMenu * menu_color_list;
  QMenu * menu_redim;
  QMenu * menu_view;
  QMenu * menu_dock;
  QMenu * menu_layer;
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

  int color_position(const QColor &) const;

  void add_recent_color(const QColor &);

  void move_color_to_begin(int);

  void redim(size_t, size_t);

  void update_actions_redim();

public:
  MainWindow(QWidget * parent = nullptr);

  void closeEvent(QCloseEvent *) override;

  void add_layer();

  void remove_layer(LayerSet::LayerVector::size_type);

  bool is_saved() const;

  void set_saved_state(bool);

  void set_action_remove_layer_enability(bool);

public slots:
  void slot_new();

  void slot_save();

  void slot_save_as();

  void slot_open();

  void slot_export();

  void slot_set_color(QColor);

  void slot_change_background_color(QColor);

  void slot_set_recent_color();

  void slot_pick_color();

  void slot_redim();

  void slot_custom_redim();

  void slot_show_dock_layers();

  void slot_close_dock_layers();

  void slot_update_zoom(double);

  void slot_new_layer();

  void slot_remove_layer();

  void slot_about();

  void slot_about_qt();

  void slot_painted(QList<std::tuple<QColor, size_t, size_t, size_t>>);

  void slot_can_undo(bool);

  void slot_can_redo(bool);

  void slot_change_layer_visibility(bool, int);

  void slot_change_layer_name(QString, int);

  void slot_change_selected_layer(int);

  void slot_update_position(size_t, size_t);
};

# endif // MAINWINDOW_H
