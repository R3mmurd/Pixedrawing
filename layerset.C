/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <layerset.H>

# include <QPainter>
# include <QFile>

QColor LayerSet::get_background_color() const
{
  return background_color;
}

void LayerSet::set_background_color(const QColor &value)
{
  background_color = value;
}

LayerSet::LayerSet(size_t r, size_t c, QColor bgcolor)
  : rows(r), cols(c), background_color(bgcolor)
{
  layers.push_back(Layer(rows, cols, "Default"));
}

LayerSet::~LayerSet()
{
  clear();
}

void LayerSet::clear()
{
  layers.clear();
}

void LayerSet::redim(size_t r, size_t c)
{
  size_t _r = std::min(r, rows);
  size_t _c = std::min(c, cols);

  for (Layer & l : layers)
    {
      QColor ** tmp = allocate_lattice(r, c);

      for (size_t i = 0; i < _r; ++i)
        for (size_t j = 0; j < _c; ++j)
          tmp[i][j] = l.mat[i][j];

      free_lattice(l.mat, rows);
      l.mat = tmp;
    }

  rows = r;
  cols = c;
}

const size_t & LayerSet::get_rows() const
{
  return rows;
}

const size_t & LayerSet::get_cols() const
{
  return cols;
}

LayerSet::LayerVector::size_type LayerSet::get_num_layers() const
{
  return layers.size();
}

void LayerSet::save_to_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::WriteOnly))
    throw std::runtime_error("Cannot create file");

  LayerVector::size_type num_layers = layers.size();

  file.write(reinterpret_cast<char *>(&num_layers),sizeof(LayerVector::size_type));
  file.write(reinterpret_cast<char *>(&rows), sizeof(size_t));
  file.write(reinterpret_cast<char *>(&cols), sizeof(size_t));

  for (Layer & l : layers)
    {
      QString::size_type sz = l.name.size() + 1;
      file.write(reinterpret_cast<char *>(&sz), sizeof(QString::size_type));
      file.write(l.name.toStdString().c_str(), sz);
      file.write(reinterpret_cast<char *>(&l.visible), sizeof(bool));
      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
        {
          int c = l.mat[i][j].red();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].green();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].blue();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
          c = l.mat[i][j].alpha();
          file.write(reinterpret_cast<char *>(&c), sizeof(int));
        }
    }

  file.close();
}

void LayerSet::load_from_file(QString & filename)
{
  QFile file(filename);

  if (not file.open(QIODevice::ReadOnly))
    throw std::runtime_error("File doesn't exist");

  clear();

  LayerVector::size_type num_layers;

  file.read(reinterpret_cast<char *>(&num_layers), sizeof(LayerVector::size_type));
  file.read(reinterpret_cast<char *>(&rows), sizeof(size_t));
  file.read(reinterpret_cast<char *>(&cols), sizeof(size_t));

  for (LayerVector::size_type l = 0; l < num_layers; ++l)
    {
      Layer layer;

      layer.rows = rows;
      layer.cols = cols;

      QString::size_type sz;
      file.read(reinterpret_cast<char *>(&sz), sizeof(QString::size_type));
      char str[sz];
      file.read(str, sz);
      layer.name = str;
      file.read(reinterpret_cast<char *>(&layer.visible), sizeof(bool));

      layer.mat = allocate_lattice(rows, cols);


      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
          {
            int c;
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setRed(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setGreen(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setBlue(c);
            file.read(reinterpret_cast<char *>(&c), sizeof(int));
            layer.mat[i][j].setAlpha(c);
          }

        layers.push_back(layer);
      }

  file.close();
}

QImage LayerSet::export_bitmap()
{
  QImage ret_val(cols, rows, QImage::Format_ARGB32);

  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
       ret_val.setPixelColor(j, i, background_color);

  for (LayerVector::size_type l = layers.size(); l > 0; --l)
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j)
        if (layers[l-1].mat[i][j] != Qt::transparent)
          ret_val.setPixelColor(j, i, layers[l-1].mat[i][j]);

  return ret_val;
}

void LayerSet::paint(size_t l, size_t i, size_t j, const QColor & c)
{
  layers[l].mat[i][j] = c;
}

const QColor & LayerSet::get_color(size_t l, size_t i, size_t j) const
{
  return layers[l].mat[i][j];
}

void LayerSet::draw(QPainter & painter, double w, double h)
{
  painter.fillRect(0, 0, cols*w, rows*h, background_color);
  for (LayerVector::size_type l = layers.size(); l > 0; --l)
    {
      if (not layers[l-1].visible)
        continue;

      for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
          painter.fillRect(j * w, i * h, w, h, layers[l-1].mat[i][j]);
    }
}

void LayerSet::show_layer(LayerVector::size_type l)
{
  if (layers[l].visible)
    return;

  layers[l].visible = true;
  emit signal_change_visibility(true);
}

void LayerSet::hide_layer(LayerVector::size_type l)
{
  if (not layers[l].visible)
    return;

  layers[l].visible = false;
  emit signal_change_visibility(false);
}

bool LayerSet::is_layer_visible(LayerVector::size_type l) const
{
  return layers[l].visible;
}

const QString &
LayerSet::get_layer_name(LayerSet::LayerVector::size_type l) const
{
  return layers[l].name;
}

void LayerSet::set_layer_name(LayerSet::LayerVector::size_type l,
                                    const QString & n)
{
  layers[l].name = n;
}

void LayerSet::add_new_layer()
{
  push_layer();
}

void LayerSet::push_layer()
{
  QString name = "layer_";
  name.append(QString::number(layers.size()));
  layers.push_front(Layer(rows, cols, name));
}

void LayerSet::pop_layer()
{
  layers.pop_front();
}

void LayerSet::insert_layer(const Layer & layer, LayerVector::size_type p)
{
  auto i = layers.size();
  layers.push_back(layer);

  while (i > 0 and i > p)
    {
      std::swap(layers[i-1], layers[i]);
      --i;
    }
}

Layer LayerSet::remove_layer(LayerSet::LayerVector::size_type l)
{
  for ( ; l < layers.size() - 1; ++l)
    std::swap(layers[l], layers[l + 1]);

  Layer ret_val = std::move(layers.back());
  layers.pop_back();
  return ret_val;
}

Layer::Layer()
{
  // empty, dummy constuctor
}

Layer::Layer(size_t r, size_t c, const QString & n)
  : rows(r), cols(c), mat(allocate_lattice(rows, cols)), name(n)
{
  // empty
}

Layer::Layer(Layer && l)
  : rows(0), cols(0), mat(nullptr), name(""), visible(false)
{
  std::swap(rows, l.rows);
  std::swap(cols, l.cols);
  std::swap(mat, l.mat);
  std::swap(name, l.name);
  std::swap(visible, l.visible);
}

Layer::Layer(const Layer & l)
  : rows(l.rows), cols(l.cols), mat(copy_lattice(l.mat, rows, cols)),
    name(l.name), visible(l.visible)
{
  // empty
}

Layer::~Layer()
{
  free_lattice(mat, rows);
}

Layer & Layer::operator = (const Layer & l)
{
  if (&l == this)
    return *this;

  free_lattice(mat, rows);

  rows = l.rows;
  cols = l.cols;
  mat = copy_lattice(l.mat, rows, cols);
  name = l.name;
  visible = l.visible;

  return *this;
}

Layer & Layer::operator = (Layer && l)
{
  std::swap(rows, l.rows);
  std::swap(cols, l.cols);
  std::swap(mat, l.mat);
  std::swap(name, l.name);
  std::swap(visible, l.visible);
  return *this;
}
