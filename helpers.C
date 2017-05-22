# include <helpers.H>

const char * DftValues::FILE_EXT = "pxdr";

QColor ** allocate_lattice(size_t rows, size_t cols, const QColor & dft_color)
{
  QColor ** lattice = new QColor*[rows];

  for (size_t i = 0; i < rows; ++i)
    {
      lattice[i] = new QColor[cols];

      for (size_t j = 0; j < cols; ++j)
        lattice[i][j] = dft_color;
    }

  return lattice;
}

void free_lattice(QColor ** lattice, size_t rows)
{
  for (size_t i = 0; i < rows; ++i)
    delete [] lattice[i];
  delete [] lattice;
}
