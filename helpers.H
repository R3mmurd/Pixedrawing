# ifndef HELPERS_H
# define HELPERS_H

# include <QColor>

struct DftValues
{
  static constexpr size_t DRAWING_PANEL_SIZE = 32;
};

QColor ** allocate_lattice(size_t, size_t);

void free_lattice(QColor **, size_t);

# endif // HELPERS_H
