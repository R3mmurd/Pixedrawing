# include <helpers.H>

QColor ** allocate_lattice(size_t rows, size_t cols)
{
  QColor ** lattice = new QColor*[rows];

  for (size_t i = 0; i < rows; ++i)
    {
      lattice[i] = new QColor[cols];

      for (size_t j = 0; j < cols; ++j)
        lattice[i][j] = Qt::transparent;
    }

  return lattice;
}

void free_lattice(QColor ** lattice, size_t rows)
{
  for (size_t i = 0; i < rows; ++i)
    delete [] lattice[i];
  delete [] lattice;
}
