# include <undocommands.H>
# include <drawingpanel.H>
# include <docklayers.H>
# include <mainwindow.H>

Paint::Paint(DrawingPanel * p,
             const QList<std::tuple<QColor, size_t, size_t, size_t> > & pc)
  : panel(p), painted_cells(pc)
{
  // empty
}

void Paint::undo()
{
  has_undo = true;

  for (auto & t : painted_cells)
    {
      QColor c = panel->get_lattice().get_color(std::get<1>(t), std::get<2>(t),
                                    std::get<3>(t));
      panel->get_lattice().paint(std::get<1>(t), std::get<2>(t), std::get<3>(t),
                   std::get<0>(t));
      std::get<0>(t) = c;
    }

  panel->repaint();
}

void Paint::redo()
{
  if (not has_undo)
    return;

  undo();
}

AddLayer::AddLayer(MainWindow * _w)
  : w(_w)
{
  // empty
}

void AddLayer::undo()
{
  w->remove_layer(0);
  has_undo = true;
}

void AddLayer::redo()
{
  if (not has_undo)
    return;

  w->add_layer();
}

