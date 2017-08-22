# ifndef UNDOCOMMANDS_H
# define UNDOCOMMANDS_H

# include <QUndoStack>
# include <QColor>
# include <QList>

class MainWindow;
class DrawingPanel;
class LayerSet;
class DockLayers;
struct Layer;

class Paint : public QUndoCommand
{
  MainWindow * w;
  bool save_state;
  DrawingPanel * panel;
  QList<std::tuple<QColor, size_t,size_t, size_t>> painted_cells;

  bool has_undo = false;

public:
  Paint(MainWindow *, bool, DrawingPanel *,
        const QList<std::tuple<QColor, size_t, size_t, size_t>> &);

  void undo() override;

  void redo() override;
};

class AddLayer : public QUndoCommand
{
  MainWindow * w;
  bool saved_state;
  DrawingPanel * panel;
  DockLayers * dock;

  bool has_undo = false;

public:
  AddLayer(MainWindow *, bool, DrawingPanel *, DockLayers *);

  void undo() override;

  void redo() override;
};

class RemoveLayer : public QUndoCommand
{
  MainWindow * w;
  bool saved_state;
  DrawingPanel * panel;
  DockLayers * dock;
  Layer * layer;
  int position;

  bool has_undo = false;

public:
  RemoveLayer(MainWindow *, bool, DrawingPanel *, DockLayers *, const Layer &,
              int);

  ~RemoveLayer();

  void undo() override;

  void redo() override;
};

class ChangeLayerName : public QUndoCommand
{
  MainWindow * w;
  bool saved_state;
  DrawingPanel * panel;
  DockLayers * dock;
  QString name;
  int position;

  bool has_undo = false;

public:
  ChangeLayerName(MainWindow *, bool, DrawingPanel *, DockLayers *,
                  const QString &, int);

  void undo() override;

  void redo() override;
};

# endif // UNDOCOMMANDS_H
