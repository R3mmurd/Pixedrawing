# include <undocommands.H>
# include <drawingpanel.H>

Paint::Paint(DrawingPanel * p,
             const QList<std::tuple<QColor, size_t, size_t> > & pc)
  : panel(p), painted_cells(pc)
{
  // empty
}

void Paint::undo()
{
  has_undo = true;

  for (auto & t : painted_cells)
    {
      QColor c = panel->get_color(std::get<1>(t), std::get<2>(t));
      panel->paint(std::get<1>(t), std::get<2>(t), std::get<0>(t));
      std::get<0>(t) = c;
    }
}

void Paint::redo()
{
  if (not has_undo)
    return;

  undo();
}
