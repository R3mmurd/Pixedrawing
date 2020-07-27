/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <docklayers.H>

# include <QCloseEvent>
# include <QHeaderView>
# include <QPainter>
# include <QMouseEvent>

# include <QDebug>

void CheckWidget::mousePressEvent(QMouseEvent * evt)
{
  checked = not checked;
  emit signal_change_visibility(checked, mapToParent(evt->pos()));
  repaint();
}

void CheckWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.fillRect(0, 0, width(), height(), Qt::white);

  QPixmap eye;

  if (checked)
    eye.load(":/icon/eye");
  else
    eye.load(":/icon/xeye");

  painter.drawPixmap(0, 0, eye);
}

CheckWidget::CheckWidget(bool c, QWidget * parent)
  : QWidget(parent), checked(c)
{
  setFixedSize(32, 32);
}

bool CheckWidget::is_checked() const
{
  return checked;
}

void DockLayers::select_row(int r)
{
  selected_row = r;
  table->selectRow(selected_row);
  emit signal_change_selected_layer(selected_row);
}

void DockLayers::closeEvent(QCloseEvent * evt)
{
  emit signal_close();
  evt->ignore();
}

DockLayers::DockLayers(QWidget * parent)
  : QDockWidget(parent)
{
  setWindowTitle("Layers");

  table = new QTableWidget(0, 2, this);
  table->horizontalHeader()->hide();
  table->verticalHeader()->hide();
  table->setColumnWidth(0, 34);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);


  setWidget(table);

  connect(table, SIGNAL(cellChanged(int,int)), +
          this, SLOT(slot_change_name(int,int)));

  connect(table, SIGNAL(itemSelectionChanged()),
          this, SLOT(slot_item_changed()));
}

void DockLayers::add_layer_info(const QList<std::tuple<QString, bool>> & l)
{
  QListIterator<std::tuple<QString, bool>> it(l);
  it.toBack();

  while (it.hasPrevious())
    add_layer_info(it.previous());
}

void DockLayers::add_layer_info(const std::tuple<QString, bool> & info)
{
  add_layer_info(info, 0);
}

void DockLayers::add_layer_info(const std::tuple<QString, bool> & info, int r)
{
  table->insertRow(r);

  CheckWidget * check_widget = new CheckWidget(std::get<1>(info), table);
  table->setCellWidget(r, 0, check_widget);
  connect(check_widget, SIGNAL(signal_change_visibility(bool,QPoint)),
          this, SLOT(slot_change_visibility(bool,QPoint)));
  table->setItem(r, 1, new QTableWidgetItem(std::get<0>(info)));
  select_row(r);
}

void DockLayers::set_layer_name(const QString & name, int row)
{
  table->item(row, 1)->setText(name);
}

void DockLayers::remove_layer_info(int r)
{
  int s = selected_row;

  table->removeRow(r);

  if (s >= table->rowCount())
    s = table->rowCount() - 1;

  select_row(s);
}

void DockLayers::clear()
{
  table->setRowCount(0);
}

void DockLayers::slot_item_changed()
{
  const QList<QTableWidgetItem *> & selected_items = table->selectedItems();

  if (selected_items.isEmpty())
    return;

  int row = table->row(selected_items.first());
  select_row(row);
}

void DockLayers::slot_change_visibility(bool v, QPoint p)
{
  int row = table->rowAt(p.y());
  emit signal_change_visibility(v, row);
}

void DockLayers::slot_change_name(int r, int c)
{
  if (c != 1)
    return;

  emit signal_change_name(table->item(r, c)->text(), r);
}
