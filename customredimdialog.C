/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# include <customredimdialog.H>

CustomRedimDialog::CustomRedimDialog(QWidget * parent)
  : QDialog(parent)
{
  ui.setupUi(this);

  validator = new QIntValidator;
  validator->setRange(0, 1024);
  ui.edt_cols->setValidator(validator);
  ui.edt_rows->setValidator(validator);
  ui.edt_cols->setFocus();
}
