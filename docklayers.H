/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
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

  void add_layer_info(const std::tuple<QString, bool> &, int);

  void set_layer_name(const QString &, int);

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
