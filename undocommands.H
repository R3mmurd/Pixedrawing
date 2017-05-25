# ifndef UNDOCOMMANDS_H
# define UNDOCOMMANDS_H

# include <QUndoStack>
# include <QColor>
# include <QList>

class MainWindow;
class DrawingPanel;
class DrawingLattice;
class DockLayers;
struct Layer;

class Paint : public QUndoCommand
{
  DrawingPanel * panel;
  QList<std::tuple<QColor, size_t,size_t, size_t>> painted_cells;

  bool has_undo = false;

public:
  Paint(DrawingPanel *,
        const QList<std::tuple<QColor, size_t, size_t, size_t>> &);

  void undo() override;

  void redo() override;
};

// TODO
class AddLayer : public QUndoCommand
{
  MainWindow * w;

  bool has_undo = false;

public:
  AddLayer(MainWindow *);

  void undo() override;

  void redo() override;
};

# endif // UNDOCOMMANDS_H
