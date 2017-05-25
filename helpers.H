# ifndef HELPERS_H
# define HELPERS_H

# include <QColor>

struct DftValues
{
  static const     char * FILE_EXT;
  static constexpr size_t NUM_DFT_REDIM_VALUES = 6;
  static constexpr size_t NUM_RECENT_COLORS    = 10;
  static constexpr int    STATUS_BAR_TIME      = 3000;
  static constexpr size_t DRAWING_PANEL_SIZE   = 32;
};

QColor ** allocate_lattice(size_t, size_t,
                           const QColor & dft_color = Qt::transparent);

void free_lattice(QColor **, size_t);

QColor ** copy_lattice(QColor **, size_t, size_t);

# endif // HELPERS_H
