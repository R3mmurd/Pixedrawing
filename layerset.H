/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# ifndef LAYERSET_H
# define LAYERSET_H

# include <QColor>
# include <QVector>
# include <QImage>
# include <QObject>

# include <helpers.H>

struct Layer
{
  size_t rows = 0;
  size_t cols = 0;
  QColor ** mat = nullptr;
  QString name = "";
  bool visible = true;

  Layer();

  Layer(size_t, size_t, const QString &);

  Layer(const Layer &);

  Layer(Layer &&);

  ~Layer();

  Layer & operator = (const Layer &);

  Layer & operator = (Layer &&);
};

class LayerSet : public QObject
{
  Q_OBJECT

public:
  using LayerVector = QVector<Layer>;

private:
  size_t rows;
  size_t cols;

  QColor background_color;

  LayerVector layers;

public:
  LayerSet(size_t, size_t, QColor bgcolor = Qt::white);

  ~LayerSet();

  void clear();

  void redim(size_t, size_t);

  const size_t & get_rows() const;

  const size_t & get_cols() const;

  LayerVector::size_type get_num_layers() const;

  void save_to_file(QString &);

  void load_from_file(QString &);

  QImage export_bitmap();

  void paint(size_t, size_t, size_t, const QColor &);

  const QColor & get_color(size_t, size_t, size_t) const;

  void draw(QPainter &, double, double);

  void show_layer(LayerVector::size_type);

  void hide_layer(LayerVector::size_type);

  bool is_layer_visible(LayerVector::size_type) const;

  const QString & get_layer_name(LayerVector::size_type) const;

  void set_layer_name(LayerVector::size_type, const QString &);

  void add_new_layer();

  void push_layer();

  void pop_layer();

  void insert_layer(const Layer &, LayerVector::size_type);

  Layer remove_layer(LayerVector::size_type);

  QColor get_background_color() const;

  void set_background_color(const QColor &);

signals:
  void signal_change_visibility(bool);

  void signal_layer_added(Layer *, int);

  void signal_layer_removed(Layer, int);
};

# endif // LAYERSET_H
