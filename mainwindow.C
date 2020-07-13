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
# include <mainwindow.H>

# include <QMenuBar>
# include <QMessageBox>
# include <QColorDialog>
# include <QFileDialog>
# include <QStatusBar>

# include <helpers.H>

void MainWindow::init_actions()
{
  action_new = new QAction(QIcon(":icon/new"), "&New", this);
  connect(action_new, SIGNAL(triggered(bool)), this, SLOT(slot_new()));
  action_new->setShortcut(tr("ctrl+n"));
  action_save = new QAction(QIcon(":icon/save"), "&Save", this);
  action_save->setShortcut(tr("ctrl+s"));
  connect(action_save, SIGNAL(triggered(bool)), this, SLOT(slot_save()));
  action_save_as = new QAction("Save as", this);
  connect(action_save_as, SIGNAL(triggered(bool)), this, SLOT(slot_save_as()));
  action_open = new QAction(QIcon(":icon/open"), "&Open", this);
  action_open->setShortcut(tr("ctrl+o"));
  connect(action_open, SIGNAL(triggered(bool)), this, SLOT(slot_open()));
  action_export = new QAction("E&xport to", this);
  connect(action_export, SIGNAL(triggered(bool)), this, SLOT(slot_export()));
  action_exit = new QAction(QIcon(":icon/exit"), "&Exit", this);
  action_exit->setShortcut(tr("ctrl+q"));
  connect(action_exit, SIGNAL(triggered(bool)), this, SLOT(close()));

  action_undo = new QAction(QIcon(":/icon/undo"), "&Undo", this);
  action_undo->setShortcut(tr("ctrl+z"));
  action_undo->setEnabled(false);
  connect(action_undo, SIGNAL(triggered(bool)), &undo_stack, SLOT(undo()));

  connect(&undo_stack, SIGNAL(canUndoChanged(bool)),
          this, SLOT(slot_can_undo(bool)));

  action_redo= new QAction(QIcon(":/icon/redo"), "&Redo", this);
  action_redo->setShortcut(tr("ctrl+y"));
  action_redo->setEnabled(false);
  connect(action_redo, SIGNAL(triggered(bool)), &undo_stack, SLOT(redo()));

  connect(&undo_stack, SIGNAL(canRedoChanged(bool)),
          this, SLOT(slot_can_redo(bool)));

  unsigned redim_value = 8;

  for (size_t i = 0; i < DftValues::NUM_DFT_REDIM_VALUES; ++i)
    {
      QString action_text = QString::asprintf("%u x %u",
                                              redim_value, redim_value);

      actions_redim[i] = new QAction(action_text, this);
      connect(actions_redim[i], SIGNAL(triggered(bool)),
              this, SLOT(slot_redim()));
      actions_redim[i]->setData(redim_value);
      actions_redim[i]->setCheckable(true);
      if (redim_value == DftValues::DRAWING_PANEL_SIZE)
        actions_redim[i]->setChecked(true);
      redim_value = redim_value << 1;
    }

  actions_redim[DftValues::NUM_DFT_REDIM_VALUES] = new QAction("Custom", this);
  connect(actions_redim[DftValues::NUM_DFT_REDIM_VALUES],
      SIGNAL(triggered(bool)), this, SLOT(slot_custom_redim()));

  actions_pick_color[0] = new QAction("Pick color", this);
  connect(actions_pick_color[0], SIGNAL(triggered(bool)),
          this, SLOT(slot_pick_color()));

  for (size_t i = 1; i <= DftValues::NUM_RECENT_COLORS; ++i)
    {
      actions_pick_color[i] = new QAction(this);
      actions_pick_color[i]->setVisible(false);
      connect(actions_pick_color[i], SIGNAL(triggered(bool)),
              this, SLOT(slot_set_recent_color()));
    }

  action_change_background_color = new QAction("Change background color", this);
  connect(action_change_background_color, SIGNAL(triggered(bool)),
          this, SLOT(slot_pick_color()));

  action_show_dock_layers = new QAction("Layers", this);
  action_show_dock_layers->setCheckable(true);
  action_show_dock_layers->setChecked(true);
  connect(action_show_dock_layers, SIGNAL(triggered(bool)),
          this, SLOT(slot_show_dock_layers()));
  action_zoom_in = new QAction(QIcon(":icon/zoom-in"), "Zoom in", this);
  action_zoom_in->setShortcut(tr("ctrl++"));
  connect(action_zoom_in, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_in()));
  action_zoom_out = new QAction(QIcon(":icon/zoom-out"), "Zoom out", this);
  action_zoom_out->setShortcut(tr("ctrl+-"));
  connect(action_zoom_out, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_out()));
  action_zoom_1 = new QAction(QIcon(":icon/zoom-1"), "Original size", this);
  action_zoom_1->setShortcut(tr("ctrl+0"));
  connect(action_zoom_1, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_1()));

  action_new_layer = new QAction("New layer", this);
  connect(action_new_layer, SIGNAL(triggered(bool)),
          this, SLOT(slot_new_layer()));
  action_remove_layer = new QAction("Remove layer", this);
  connect(action_remove_layer, SIGNAL(triggered(bool)),
          this, SLOT(slot_remove_layer()));

  action_about = new QAction("About", this);
  connect(action_about, SIGNAL(triggered(bool)), this, SLOT(slot_about()));

  action_about_qt = new QAction("About Qt", this);
  connect(action_about_qt, SIGNAL(triggered(bool)),
          this, SLOT(slot_about_qt()));

  position_label = new QLabel;
  statusBar()->addPermanentWidget(position_label);
}

void MainWindow::init_menu()
{
  menu_file = new QMenu("&File", this);
  menu_file->addAction(action_new);
  menu_file->addAction(action_open);
  menu_file->addAction(action_save);
  menu_file->addAction(action_save_as);
  menu_file->addAction(action_export);
  menu_file->addSeparator();
  menu_file->addAction(action_exit);
  menuBar()->addMenu(menu_file);

  menu_edit = new QMenu("&Edit", this);

  menu_edit->addAction(action_undo);
  menu_edit->addAction(action_redo);

  menu_color_list = new QMenu("Select color", this);
  menu_color_list->addAction(actions_pick_color[0]);
  menu_color_list->addSeparator();

  for (size_t i = 1; i <= DftValues::NUM_RECENT_COLORS; ++i)
    menu_color_list->addAction(actions_pick_color[i]);

  menu_edit->addMenu(menu_color_list);

  menu_redim = new QMenu("Resize", this);

  for (unsigned i = 0; i < DftValues::NUM_DFT_REDIM_VALUES; ++i)
    menu_redim->addAction(actions_redim[i]);

  menu_redim->addAction(actions_redim[DftValues::NUM_DFT_REDIM_VALUES]);

  menu_edit->addMenu(menu_redim);

  menu_edit->addSeparator();
  menu_edit->addAction(action_change_background_color);

  menuBar()->addMenu(menu_edit);

  menu_view = new QMenu("&View", this);

  menu_dock = new QMenu("Dock widgets", this);
  menu_dock->addAction(action_show_dock_layers);
  menu_view->addMenu(menu_dock);

  menu_view->addAction(action_zoom_in);
  menu_view->addAction(action_zoom_out);
  menu_view->addAction(action_zoom_1);
  menuBar()->addMenu(menu_view);

  menu_layer = new QMenu("&Layer", this);
  menu_layer->addAction(action_new_layer);
  menu_layer->addAction(action_remove_layer);
  menuBar()->addMenu(menu_layer);

  menu_help = new QMenu("&Help", this);
  menu_help->addAction(action_about);
  menu_help->addAction(action_about_qt);
  menuBar()->addMenu(menu_help);
}

void MainWindow::init_toolbar()
{
  tool_bar = addToolBar("Toolbar");

  tool_bar->addAction(action_new);
  tool_bar->addAction(action_save);
  tool_bar->addAction(action_open);
  tool_bar->addAction(action_exit);
  tool_bar->addSeparator();
  tool_bar->addAction(action_undo);
  tool_bar->addAction(action_redo);
  tool_bar->addSeparator();
  tool_bar->addAction(action_zoom_in);
  tool_bar->addAction(action_zoom_out);
  tool_bar->addAction(action_zoom_1);
}

void MainWindow::init_gui()
{
  dock_layers = new DockLayers(this);
  addDockWidget(Qt::RightDockWidgetArea, dock_layers);
  connect(dock_layers, SIGNAL(signal_close()),
          this, SLOT(slot_close_dock_layers()));
  connect(dock_layers, SIGNAL(signal_change_name(QString,int)),
          this, SLOT(slot_change_layer_name(QString,int)));
  connect(dock_layers, SIGNAL(signal_change_visibility(bool,int)),
          this, SLOT(slot_change_layer_visibility(bool,int)));
  connect(dock_layers, SIGNAL(signal_change_selected_layer(int)),
          this, SLOT(slot_change_selected_layer(int)));

  create_work();
  dock_layers->add_layer_info(drawing_panel->get_layer_info());
  statusBar()->showMessage("Creating actions...");
  init_actions();
  statusBar()->showMessage("Creating menus...");
  init_menu();
  statusBar()->showMessage("Creating toolbar...");
  init_toolbar();
  statusBar()->showMessage("Creating workspace...");

  resize(800, 600);
  setWindowIcon(QIcon(":icon/app-icon"));

  statusBar()->showMessage("GUI created successfully",
                           DftValues::STATUS_BAR_TIME);
}

void MainWindow::save_file()
{
  try
  {
    statusBar()->showMessage("Saving file...");
    drawing_panel->save_to_file(name);
    saved = true;
    statusBar()->showMessage("File saved successfully",
                             DftValues::STATUS_BAR_TIME);
  }
  catch(const std::exception & e)
  {
    QMessageBox::critical(this, "Error saving file", e.what());
    statusBar()->showMessage("Error saving file",
                             DftValues::STATUS_BAR_TIME);
  }
}

void MainWindow::set_title()
{
  QString title;

  if (not saved)
    title.append("*");

  title.append(QString::asprintf("Pixedrawing [%lu x %lu]",
                                 drawing_panel->get_cols(),
                                 drawing_panel->get_rows()));

  title.append(" (");

  if (not name.isEmpty())
    {
      QFileInfo file_info(name);
      title.append(file_info.baseName());
      title.append(".");
      title.append(file_info.suffix());
    }
  else
    title.append("New work");

  title.append(")");

  setWindowTitle(title);
}

void MainWindow::create_work()
{
  drawing_panel_wrapper = new DrawingPanelWrapper(this);
  drawing_panel = drawing_panel_wrapper->get_drawing_panel();
  drawing_panel_wrapper->show();
  connect(drawing_panel_wrapper, SIGNAL(signal_zoom(double)),
          this, SLOT(slot_update_zoom(double)));
  connect(drawing_panel,
          SIGNAL(signal_painted(QList<std::tuple<QColor, size_t, size_t,
                                size_t>>)),
          this, SLOT(slot_painted(QList<std::tuple<QColor, size_t, size_t,
                                  size_t>>)));
  connect(drawing_panel, SIGNAL(signal_position(size_t, size_t)),
          this, SLOT(slot_update_position(size_t, size_t)));
  setCentralWidget(drawing_panel_wrapper);
  saved = true;
  set_title();
}

void MainWindow::close_work()
{
  if (not saved)
    {
      QString msg;
      msg.append("You have done some changes recently\n");
      msg.append("Save changes?");
      if (QMessageBox::question(this, "Security", msg,
                                QMessageBox::Yes | QMessageBox::No)
          == QMessageBox::Yes)
        slot_save();
    }
  disconnect(drawing_panel_wrapper, SIGNAL(signal_zoom(double)),
             this, SLOT(slot_update_zoom(double)));
  disconnect(drawing_panel,
             SIGNAL(signal_painted(QList<std::tuple<QColor, size_t, size_t,
                                   size_t>>)),
             this,
             SLOT(slot_painted(QList<std::tuple<QColor, size_t, size_t,
                               size_t>>)));
  dock_layers->clear();
  drawing_panel_wrapper->close();
  delete drawing_panel_wrapper;
  drawing_panel = nullptr;
  undo_stack.clear();
  action_undo->setEnabled(false);
  action_redo->setEnabled(false);
}

int MainWindow::color_position(const QColor & color) const
{
  for (int i = 1; i <= recent_colors.size(); ++i)
    if (recent_colors[actions_pick_color[i]->data().toInt()] == color)
      return i - 1;

  return -1;
}

void MainWindow::add_recent_color(const QColor & color)
{
  int pos = color_position(color);

  if (pos >= 0)
    move_color_to_begin(pos);
  else
    recent_colors.push_front(color);

  if (size_t(recent_colors.size()) > DftValues::NUM_RECENT_COLORS)
    recent_colors.pop_back();

  for (size_t i = 1; i <= DftValues::NUM_RECENT_COLORS; ++i)
    {
      if (i <= size_t(recent_colors.size()))
        {
          actions_pick_color[i]->setText(recent_colors[i - 1].name());
          actions_pick_color[i]->setData(int(i - 1));
          actions_pick_color[i]->setVisible(true);
          QPixmap color_sample(15, 15);
          color_sample.fill(recent_colors[i - 1]);
          actions_pick_color[i]->setIcon(QIcon(color_sample));
        }
      else
        actions_pick_color[i]->setVisible(false);
    }
}

void MainWindow::move_color_to_begin(int p)
{
  QColor color = recent_colors[p];

  for (int i = p; i > 0; --i)
    recent_colors[i] = recent_colors[i - 1];

  recent_colors[0] = color;
}

void MainWindow::redim(size_t width, size_t height)
{
  if (height < drawing_panel->get_rows() or width< drawing_panel->get_cols())
    {
      QString msg = "You might lost some pixels\n";
      msg.append("Do you want to continue?");

      if (QMessageBox::question(this, "Warning", msg,
                                QMessageBox::Yes | QMessageBox::No) ==
          QMessageBox::No)
        {
          statusBar()->showMessage("Resize not done!",
                                   DftValues::STATUS_BAR_TIME);
          update_actions_redim();
          return;
        }
    }

  drawing_panel->redim(width, height);
  saved = false;

  statusBar()->showMessage("Panel resized successfully",
                           DftValues::STATUS_BAR_TIME);
  update_actions_redim();
}

void MainWindow::update_actions_redim()
{
  for (size_t i = 0; i < DftValues::NUM_DFT_REDIM_VALUES; ++i)
    {
      if (drawing_panel->get_rows() == drawing_panel->get_cols() and
          actions_redim[i]->data().toUInt() == drawing_panel->get_rows())
        actions_redim[i]->setChecked(true);
      else
        actions_redim[i]->setChecked(false);
    }
}

MainWindow::MainWindow(QWidget * parent)
  : QMainWindow(parent)
{
  init_gui();

  last_visited_path = QDir::homePath();
}

void MainWindow::closeEvent(QCloseEvent *)
{
  close_work();
}

void MainWindow::add_layer()
{
  drawing_panel->get_lattice().add_new_layer();
  dock_layers->add_layer_info(drawing_panel->get_layer_info(0));
  action_remove_layer->setEnabled(true);
  drawing_panel->repaint();
  saved = false;
}

void MainWindow::remove_layer(LayerSet::LayerVector::size_type l)
{
  Layer layer = drawing_panel->get_lattice().remove_layer(l);
  dock_layers->remove_layer_info(l);

  drawing_panel->repaint();

  if (drawing_panel->get_lattice().get_num_layers() == 0)
    action_remove_layer->setEnabled(false);

  undo_stack.push(new RemoveLayer(this, saved, drawing_panel, dock_layers,
                                  layer, l));

  saved = false;
}

bool MainWindow::is_saved() const
{
  return saved;
}

void MainWindow::set_saved_state(bool s)
{
  saved = s;
}

void MainWindow::set_action_remove_layer_enability(bool v)
{
  action_remove_layer->setEnabled(v);
}

void MainWindow::slot_new()
{
  close_work();
  create_work();
  dock_layers->add_layer_info(drawing_panel->get_layer_info());
}

void MainWindow::slot_save()
{
  if (saved)
    return;

  if (name.isEmpty())
    slot_save_as();
  else
    save_file();
}

void MainWindow::slot_save_as()
{
  QString ext = "*.";
  ext.append(DftValues::FILE_EXT);

  QString filename = QFileDialog::getSaveFileName(this, "Save file as",
                                      last_visited_path, ext);

  if (filename.isEmpty())
    {
      statusBar()->showMessage("File not saved", DftValues::STATUS_BAR_TIME);
      return;
    }

  name = filename;
  QFileInfo file_info(name);

  if (file_info.suffix() != DftValues::FILE_EXT)
    {
      name.append(".");
      name.append(DftValues::FILE_EXT);
    }

  save_file();
  set_title();

  last_visited_path = file_info.path();
}

void MainWindow::slot_open()
{
  QString ext = "*.";
  ext.append(DftValues::FILE_EXT);

  QString filename = QFileDialog::getOpenFileName(this, "Open file",
                                      last_visited_path, ext);
  if (filename.isEmpty())
    {
      statusBar()->showMessage("File not opened",
                               DftValues::STATUS_BAR_TIME);
      return;
    }

  name = filename;
  close_work();
  create_work();

  try
  {
    statusBar()->showMessage("Opening file...");
    drawing_panel->load_from_file(name);
    last_visited_path = QFileInfo(name).path();
    saved = true;
    update_actions_redim();
    statusBar()->showMessage("File opened successfully",
                             DftValues::STATUS_BAR_TIME);
  }
  catch (const std::exception & e)
  {
    QMessageBox::critical(this, "Error opening file", e.what());
    statusBar()->showMessage("Error opening file",
                             DftValues::STATUS_BAR_TIME);
  }

  dock_layers->add_layer_info(drawing_panel->get_layer_info());
}

void MainWindow::slot_export()
{
  QString filename = QFileDialog::getSaveFileName(this, "Export image as",
                                                  last_visited_path,
                                                  tr("Images (*.jpg *.png)"));

  if (filename.isEmpty())
    {
      statusBar()->showMessage("Image not exported",
                               DftValues::STATUS_BAR_TIME);
      return;
    }

  QFileInfo file_info(filename);
  QString suffix = file_info.suffix();

  if (suffix.isEmpty() or (suffix != "png" and suffix != "jpg"))
    {
      suffix = "png";
      filename.append(".");
      filename.append(suffix);
    }

  QImage img = drawing_panel->export_bitmap();
  img.save(filename, suffix.toStdString().c_str());
  statusBar()->showMessage("Image exported successfully",
                           DftValues::STATUS_BAR_TIME);
}

void MainWindow::slot_set_color(QColor c)
{
  QColor last_color = drawing_panel->get_color_to_paint();

  drawing_panel->set_color_to_paint(c);

  add_recent_color(last_color);

  QString msg = "Color changed to ";
  msg.append(c.name());

  statusBar()->showMessage(msg, DftValues::STATUS_BAR_TIME);
}

void MainWindow::slot_change_background_color(QColor c)
{
  drawing_panel->change_background_color(c);

  QString msg = "Background color changed to ";
  msg.append(c.name());

  statusBar()->showMessage(msg, DftValues::STATUS_BAR_TIME);
}

void MainWindow::slot_set_recent_color()
{
  QAction * sndr = static_cast<QAction *>(sender());
  int color_index = sndr->data().toInt();
  QColor last_color = drawing_panel->get_color_to_paint();
  drawing_panel->set_color_to_paint(recent_colors[color_index]);
  add_recent_color(last_color);
}

void MainWindow::slot_pick_color()
{
  QObject * sndr = sender();

  QColorDialog * color_dialog = new QColorDialog(this);
  color_dialog->setOption(QColorDialog::ShowAlphaChannel);

  if (sndr == action_change_background_color)
    connect(color_dialog, SIGNAL(colorSelected(QColor)),
            this, SLOT(slot_change_background_color(QColor)));
  else
    connect(color_dialog, SIGNAL(colorSelected(QColor)),
            this, SLOT(slot_set_color(QColor)));

  color_dialog->exec();
}

void MainWindow::slot_redim()
{
  QAction * sndr = static_cast<QAction *>(sender());

  unsigned redim_value = sndr->data().toUInt();
  redim(redim_value, redim_value);
}

void MainWindow::slot_custom_redim()
{
  CustomRedimDialog custom_redim_dialog(this);
  custom_redim_dialog.setModal(true);

  if (not custom_redim_dialog.exec())
    return;

  if (custom_redim_dialog.ui.edt_cols->text().isEmpty() or
      custom_redim_dialog.ui.edt_rows->text().isEmpty())
    return;

  redim(custom_redim_dialog.ui.edt_cols->text().toULong(),
        custom_redim_dialog.ui.edt_rows->text().toULong());
}

void MainWindow::slot_show_dock_layers()
{
  if (action_show_dock_layers->isChecked())
    dock_layers->show();
  else
    dock_layers->hide();
}

void MainWindow::slot_close_dock_layers()
{
  dock_layers->hide();
  action_show_dock_layers->setChecked(false);
}

void MainWindow::slot_update_zoom(double factor)
{
  action_zoom_in->setEnabled(factor <= DrawingPanelWrapper::MAX_ZOOM_FACTOR);
  action_zoom_out->setEnabled(factor >= DrawingPanelWrapper::MIN_ZOOM_FACTOR);
}

void MainWindow::slot_new_layer()
{
  undo_stack.push(new AddLayer(this, saved, drawing_panel, dock_layers));
  add_layer();
}

void MainWindow::slot_remove_layer()
{
  int l = drawing_panel->get_current_layer();
  remove_layer(l);
}

void MainWindow::slot_about()
{
  QMessageBox::about(this, "About Pixedrawing", "");
}

void MainWindow::slot_about_qt()
{
  QMessageBox::aboutQt(this, "Acerca de Qt");
}

void MainWindow::slot_painted(
    QList<std::tuple<QColor, size_t, size_t, size_t>> l)
{
  undo_stack.push(new Paint(this, saved, drawing_panel, l));
  saved = false;
  set_title();
}

void MainWindow::slot_can_undo(bool value)
{
  action_undo->setEnabled(value);
}

void MainWindow::slot_can_redo(bool value)
{
  action_redo->setEnabled(value);
}

void MainWindow::slot_change_layer_visibility(bool value, int l)
{
  if (drawing_panel->get_lattice().is_layer_visible(l) == value)
    return;

  if (value)
    drawing_panel->get_lattice().show_layer(l);
  else
    drawing_panel->get_lattice().hide_layer(l);

  saved = false;
}

void MainWindow::slot_change_layer_name(QString name, int l)
{
  if (drawing_panel->get_lattice().get_layer_name(l) == name)
    return;

  QString n = std::get<0>(drawing_panel->get_layer_info(l));

  undo_stack.push(new ChangeLayerName(this, saved, drawing_panel, dock_layers,
                                      n, l));

  drawing_panel->get_lattice().set_layer_name(l, name);

  saved = false;
}

void MainWindow::slot_change_selected_layer(int l)
{
  drawing_panel->set_current_layer(l);
}

void MainWindow::slot_update_position(size_t r , size_t c)
{
  QString position_text = "row: ";
  position_text.append(QString().setNum(r));
  position_text.append("; col: ");
  position_text.append(QString().setNum(c));
  position_label->setText(position_text);
}
