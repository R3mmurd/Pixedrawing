/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <undocommands.H>
# include <drawingpanel.H>
# include <docklayers.H>
# include <mainwindow.H>

Paint::Paint(MainWindow * _w, bool s, DrawingPanel * p,
             const QList<std::tuple<QColor, size_t, size_t, size_t> > & pc)
  : w(_w), save_state(s), panel(p), painted_cells(pc)
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

  bool s = w->is_saved();
  w->set_saved_state(save_state);
  save_state = s;
}

void Paint::redo()
{
  if (not has_undo)
    return;

  undo();
}

AddLayer::AddLayer(MainWindow * _w, bool s, DrawingPanel * p, DockLayers * d)
  : w(_w), saved_state(s), panel(p), dock(d)
{
  // empty
}

void AddLayer::undo()
{
  panel->get_lattice().remove_layer(0);
  dock->remove_layer_info(0);
  panel->repaint();

  w->set_saved_state(saved_state);

  if (panel->get_lattice().get_num_layers() == 0)
    w->set_action_remove_layer_enability(false);

  has_undo = true;
}

void AddLayer::redo()
{
  if (not has_undo)
    return;

  w->add_layer();
}

RemoveLayer::RemoveLayer(MainWindow * _w, bool s, DrawingPanel * p,
                         DockLayers * d, const Layer & l, int pos)
  : w(_w), saved_state(s), panel(p), dock(d), layer(new Layer(l)), position(pos)
{
  // empty
}

RemoveLayer::~RemoveLayer()
{
  delete layer;
}

void RemoveLayer::undo()
{
  panel->get_lattice().insert_layer(*layer, position);
  dock->add_layer_info(panel->get_layer_info(position), position);
  panel->repaint();
  w->set_saved_state(saved_state);
  w->set_action_remove_layer_enability(true);
  has_undo = true;
}

void RemoveLayer::redo()
{
  if (not has_undo)
    return;

  panel->get_lattice().remove_layer(position);
  dock->remove_layer_info(position);
  panel->repaint();
  w->set_saved_state(false);
  if (panel->get_lattice().get_num_layers() == 0)
    w->set_action_remove_layer_enability(false);
}

ChangeLayerName::ChangeLayerName(MainWindow * _w, bool s, DrawingPanel * p,
                                 DockLayers * d, const QString & n, int pos)
  : w(_w), saved_state(s), panel(p), dock(d), name(n), position(pos)
{
  // empty
}

void ChangeLayerName::undo()
{
  QString n = std::get<0>(panel->get_layer_info(position));

  panel->get_lattice().set_layer_name(position, name);
  dock->set_layer_name(name, position);

  bool s = w->is_saved();
  w->set_saved_state(saved_state);
  saved_state = s;

  name = n;

  has_undo = true;
}

void ChangeLayerName::redo()
{
  if (not has_undo)
    return;

  undo();
}
