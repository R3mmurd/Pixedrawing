/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <drawingpanel.H>

# include <QPainter>
# include <QMouseEvent>

void DrawingPanel::adjust_size()
{
  resize(lattice.get_cols() * SCALE * zoom_factor,
         lattice.get_rows() * SCALE * zoom_factor);
}

void DrawingPanel::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  double w = SCALE * zoom_factor;
  double h = SCALE * zoom_factor;

  lattice.draw(painter, w, h);

  painter.setPen(Qt::darkGray);

  for (size_t i = 0; i <= lattice.get_rows(); ++i)
    painter.drawLine(0, i * w, width(), i * h);

  for (size_t j = 0; j <= lattice.get_cols(); ++j)
    painter.drawLine(j * w, 0, j * h, height());
}

void DrawingPanel::mousePressEvent(QMouseEvent * evt)
{
  if (current_layer < 0)
    return;

  pressed_button = evt->button();

  if (pressed_button != Qt::LeftButton and pressed_button != Qt::RightButton)
    return;

  const QPoint & pos = evt->pos();

  size_t i = pos.y() / (SCALE * zoom_factor);
  size_t j = pos.x() / (SCALE * zoom_factor);

  if (i >= lattice.get_rows() or j >= lattice.get_cols())
    return;

  QColor last_color = lattice.get_color(current_layer, i, j);

  if (pressed_button == Qt::LeftButton)
    {
      if (last_color == color_to_paint)
        return;
      lattice.paint(current_layer,i,j,color_to_paint);
      painted_cells.append(std::make_tuple(last_color,current_layer,i,j));
    }
  else
    {
      if (last_color == Qt::transparent)
        return;
      lattice.paint(current_layer,i,j,Qt::transparent);
      painted_cells.append(std::make_tuple(last_color,current_layer,i,j));
    }

  repaint();
}

void DrawingPanel::mouseMoveEvent(QMouseEvent * evt)
{
  if (current_layer < 0)
    return;

  if (pressed_button != Qt::LeftButton and pressed_button != Qt::RightButton)
    return;

  const QPoint & pos = evt->pos();

  size_t i = pos.y() / (SCALE * zoom_factor);
  size_t j = pos.x() / (SCALE * zoom_factor);

  if (i >= lattice.get_rows() or j >= lattice.get_cols())
    return;

  QColor last_color = lattice.get_color(current_layer, i, j);

  if (pressed_button == Qt::LeftButton)
    {
      if (last_color == color_to_paint)
        return;
      lattice.paint(current_layer,i,j,color_to_paint);
      painted_cells.append(std::make_tuple(last_color,current_layer,i,j));
    }
  else
    {
      if (last_color == Qt::transparent)
        return;
      lattice.paint(current_layer,i,j,Qt::transparent);
      painted_cells.append(std::make_tuple(last_color,current_layer,i,j));
    }

  repaint();
}

void DrawingPanel::mouseReleaseEvent(QMouseEvent *)
{
  if (painted_cells.isEmpty())
    return;

  emit signal_painted(painted_cells);
  painted_cells.clear();
}

DrawingPanel::DrawingPanel(size_t r, size_t c, QWidget * parent)
  : QWidget(parent), lattice(r, c)
{
  adjust_size();
  connect(&lattice, SIGNAL(signal_change_visibility(bool)),
          this, SLOT(slot_change_visibility(bool)));
}

const QColor & DrawingPanel::get_color_to_paint() const
{
  return color_to_paint;
}

void DrawingPanel::set_color_to_paint(const QColor & c)
{
  color_to_paint = c;
}

void DrawingPanel::zoom_in()
{
  zoom_factor += 0.1;
  adjust_size();
  repaint();
}

void DrawingPanel::zoom_out()
{
  zoom_factor -= 0.1;
  adjust_size();
  repaint();
}

void DrawingPanel::zoom_1()
{
  zoom_factor = 1.0;
  adjust_size();
  repaint();
}

const double & DrawingPanel::get_zoom_factor() const
{
  return zoom_factor;
}

void DrawingPanel::save_to_file(QString & file_name)
{
  lattice.save_to_file(file_name);
}

void DrawingPanel::load_from_file(QString & file_name)
{
  lattice.load_from_file(file_name);
  adjust_size();
}

QImage DrawingPanel::export_bitmap()
{
  return lattice.export_bitmap();
}

const size_t & DrawingPanel::get_rows() const
{
  return lattice.get_rows();
}

const size_t & DrawingPanel::get_cols() const
{
  return lattice.get_cols();
}

void DrawingPanel::redim(size_t w, size_t h)
{
  lattice.redim(h, w);
  adjust_size();
}

LayerSet &DrawingPanel::get_lattice()
{
  return lattice;
}

void DrawingPanel::set_current_layer(LayerSet::LayerVector::size_type l)
{
  current_layer = l;
}

const LayerSet::LayerVector::size_type &
DrawingPanel::get_current_layer() const
{
  return current_layer;
}

std::tuple<QString, bool>
DrawingPanel::get_layer_info(LayerSet::LayerVector::size_type l) const
{
  return std::make_tuple(lattice.get_layer_name(l),
                         lattice.is_layer_visible(l));
}

QList<std::tuple<QString, bool>> DrawingPanel::get_layer_info() const
{
  QList<std::tuple<QString, bool>> ret_val;

  for (LayerSet::LayerVector::size_type l = 0;
       l < lattice.get_num_layers(); ++l)
    ret_val.append(get_layer_info(l));

  return ret_val;
}

void DrawingPanel::slot_change_visibility(bool)
{
  repaint();
}
