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

const char * MainWindow::EXT = "pxdr";

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

  action_zoom_in = new QAction(QIcon(":icon/zoom-in"), "Zoom in", this);
  action_zoom_in->setShortcut(tr("ctrl++"));
  connect(action_zoom_in, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_in()));
  action_zoom_out = new QAction(QIcon(":icon/zoom-out"), "Zoom out", this);
  action_zoom_out->setShortcut(tr("ctrl+-"));
  connect(action_zoom_out, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_out()));
  action_zoom_1 = new QAction(QIcon(":icon/zoom-1"), "Original size", this);
  connect(action_zoom_1, SIGNAL(triggered(bool)),
          drawing_panel_wrapper, SLOT(slot_zoom_1()));

  unsigned redim_value = 8;

  for (size_t i = 0; i < NUM_DFT_REDIM_VALUES; ++i)
    {
      QString action_text = QString::asprintf("%u x %u",
                                              redim_value, redim_value);

      actions_redim[i] = new QAction(action_text, this);
      connect(actions_redim[i], SIGNAL(triggered(bool)),
              this, SLOT(slot_redim()));
      actions_redim[i]->setData(redim_value);
      actions_redim[i]->setCheckable(true);
      if (redim_value == 64)
        actions_redim[i]->setChecked(true);
      redim_value = redim_value << 1;
    }

  actions_redim[NUM_DFT_REDIM_VALUES] = new QAction("Custom", this);
  connect(actions_redim[NUM_DFT_REDIM_VALUES], SIGNAL(triggered(bool)),
          this, SLOT(slot_custom_redim()));

  actions_pick_color[0] = new QAction("Pick color", this);
  connect(actions_pick_color[0], SIGNAL(triggered(bool)),
          this, SLOT(slot_pick_color()));

  for (size_t i = 1; i <= NUM_RECENT_COLORS; ++i)
    {
      actions_pick_color[i] = new QAction(this);
      actions_pick_color[i]->setVisible(false);
      connect(actions_pick_color[i], SIGNAL(triggered(bool)),
              this, SLOT(slot_set_recent_color()));
    }

  action_about = new QAction("About", this);
  connect(action_about, SIGNAL(triggered(bool)), this, SLOT(slot_about()));

  action_about_qt = new QAction("About Qt", this);
  connect(action_about_qt, SIGNAL(triggered(bool)),
          this, SLOT(slot_about_qt()));
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

  menu_color_list = new QMenu("Select color", this);
  menu_color_list->addAction(actions_pick_color[0]);
  menu_color_list->addSeparator();

  for (size_t i = 1; i <= NUM_RECENT_COLORS; ++i)
    menu_color_list->addAction(actions_pick_color[i]);

  menu_edit->addMenu(menu_color_list);

  menu_redim = new QMenu("Redim", this);

  for (unsigned i = 0; i < NUM_DFT_REDIM_VALUES; ++i)
    menu_redim->addAction(actions_redim[i]);

  menu_redim->addAction(actions_redim[NUM_DFT_REDIM_VALUES]);

  menu_edit->addMenu(menu_redim);
  menuBar()->addMenu(menu_edit);

  menu_view = new QMenu("&View", this);
  menu_view->addAction(action_zoom_in);
  menu_view->addAction(action_zoom_out);
  menu_view->addAction(action_zoom_1);
  menuBar()->addMenu(menu_view);

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
  tool_bar->addAction(action_zoom_in);
  tool_bar->addAction(action_zoom_out);
  tool_bar->addAction(action_zoom_1);
}

void MainWindow::init_gui()
{
  create_work();
  statusBar()->showMessage("Creating actions...");
  init_actions();
  statusBar()->showMessage("Creating menus...");
  init_menu();
  statusBar()->showMessage("Creating toolbar...");
  init_toolbar();
  statusBar()->showMessage("Creating workspace...");

  resize(800, 600);

  statusBar()->showMessage("GUI created successfully", STATUS_BAR_TIME);
}

void MainWindow::save_file()
{
  try
  {
    statusBar()->showMessage("Saving file...");
    drawing_panel->save_to_file(name);
    is_saved = true;
    statusBar()->showMessage("File saved successfully", STATUS_BAR_TIME);
  }
  catch(const std::exception & e)
  {
    QMessageBox::critical(this, "Error saving file", e.what());
    statusBar()->showMessage("Error saving file", STATUS_BAR_TIME);
  }
}

void MainWindow::set_title()
{
  QString title = QString::asprintf("Pixedrawing [%lu x %lu]",
                                    drawing_panel->get_cols(),
                                    drawing_panel->get_rows());
  if (not name.isEmpty())
    {
      QFileInfo file_info(name);
      title.append(" (");
      title.append(file_info.baseName());
      title.append(".");
      title.append(file_info.suffix());
      title.append(")");
    }

  setWindowTitle(title);
}

void MainWindow::create_work()
{
  drawing_panel_wrapper = new DrawingPanelWrapper(this);
  drawing_panel = drawing_panel_wrapper->get_drawing_panel();
  drawing_panel_wrapper->show();
  connect(drawing_panel_wrapper, SIGNAL(signal_zoom(double)),
          this, SLOT(slot_update_zoom(double)));
  connect(drawing_panel, SIGNAL(signal_changed()), this, SLOT(slot_changed()));
  setCentralWidget(drawing_panel_wrapper);
  set_title();
}

void MainWindow::close_work()
{
  if (not is_saved)
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
  disconnect(drawing_panel, SIGNAL(signal_changed()),
             this, SLOT(slot_changed()));
  drawing_panel_wrapper->close();
  delete drawing_panel_wrapper;
  drawing_panel = nullptr;
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

void MainWindow::slot_changed()
{
  is_saved = false;
}

void MainWindow::slot_new()
{
  close_work();
  create_work();
}

void MainWindow::slot_save()
{
  if (is_saved)
    return;

  if (name.isEmpty())
    slot_save_as();
  else
    save_file();
}

void MainWindow::slot_save_as()
{
  QString ext = "*.";
  ext.append(EXT);

  name = QFileDialog::getSaveFileName(this, "Save file as",
                                      last_visited_path, ext);

  if (name.isEmpty())
    {
      statusBar()->showMessage("File not saved", STATUS_BAR_TIME);
      return;
    }

  QFileInfo file_info(name);

  if (file_info.suffix() != EXT)
    {
      name.append(".");
      name.append(EXT);
    }

  save_file();
  set_title();

  last_visited_path = file_info.path();
}

void MainWindow::slot_open()
{
  QString ext = "*.";
  ext.append(EXT);

  name = QFileDialog::getOpenFileName(this, "Open file",
                                      last_visited_path, ext);
  if (name.isEmpty())
    {
      statusBar()->showMessage("File not opened", STATUS_BAR_TIME);
      return;
    }

  slot_new();

  try
  {
    statusBar()->showMessage("Opening file...");
    drawing_panel->load_from_file(name);
    last_visited_path = QFileInfo(name).path();
    statusBar()->showMessage("File opened successfully", STATUS_BAR_TIME);
  }
  catch (const std::exception & e)
  {
    QMessageBox::critical(this, "Error opening file", e.what());
    statusBar()->showMessage("Error opening file", STATUS_BAR_TIME);
  }
}

void MainWindow::slot_export()
{
  QString filename = QFileDialog::getSaveFileName(this, "Export image as",
                                                  last_visited_path,
                                                  tr("Images (*.jpg *.png)"));

  if (filename.isEmpty())
    {
      statusBar()->showMessage("Image not exported", STATUS_BAR_TIME);
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

  QImage img = drawing_panel->to_image();
  img.save(filename, suffix.toStdString().c_str());
  statusBar()->showMessage("Image exported successfully", STATUS_BAR_TIME);
}

void MainWindow::slot_set_color(QColor c)
{
  drawing_panel->set_color_to_paint(c);

  QString msg = "Color changed to ";
  msg.append(c.name());

  statusBar()->showMessage(msg, STATUS_BAR_TIME);

  recent_colors.push_front(c);

  if (size_t(recent_colors.size()) > NUM_RECENT_COLORS)
    recent_colors.pop_back();

  for (size_t i = 1; i <= NUM_RECENT_COLORS; ++i)
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

void MainWindow::slot_set_recent_color()
{
  QAction * sndr = static_cast<QAction *>(sender());
  int color_index = sndr->data().toInt();
  drawing_panel->set_color_to_paint(recent_colors[color_index]);
}

void MainWindow::slot_pick_color()
{
  QColorDialog * color_dialog = new QColorDialog(this);
  connect(color_dialog, SIGNAL(colorSelected(QColor)),
          this, SLOT(slot_set_color(QColor)));  
  color_dialog->exec();
}

void MainWindow::slot_redim()
{
  QAction * sndr = static_cast<QAction *>(sender());

  if (not sndr->isChecked())
    {
      sndr->setChecked(true);
      return;
    }

  for (size_t i = 0; i < NUM_DFT_REDIM_VALUES; ++i)
    if (actions_redim[i] != sndr)
      actions_redim[i]->setChecked(false);

  unsigned redim_value = sndr->data().toUInt();
  drawing_panel->redim(redim_value, redim_value);
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

  drawing_panel->redim(custom_redim_dialog.ui.edt_cols->text().toULong(),
                       custom_redim_dialog.ui.edt_rows->text().toULong());

  for (size_t i = 0; i < NUM_DFT_REDIM_VALUES; ++i)
    actions_redim[i]->setChecked(false);
}

void MainWindow::slot_update_zoom(double factor)
{
  action_zoom_in->setEnabled(factor <= 2.0);
  action_zoom_out->setEnabled(factor >= 0.4);
}

void MainWindow::slot_about()
{
  QMessageBox::about(this, "About Pixedrawing", "");
}

void MainWindow::slot_about_qt()
{
  QMessageBox::aboutQt(this, "Acerca de Qt");
}