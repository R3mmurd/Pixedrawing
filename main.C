/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <QApplication>
# include <mainwindow.H>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}
