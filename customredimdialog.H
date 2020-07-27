/*
  Pixedrawing.

  Autor: Alejandro Mujica (aledrums@gmail.com)
*/

# ifndef CUSTOMREDIMDIALOG_H
# define CUSTOMREDIMDIALOG_H

# include <QDialog>
# include <ui_customredimdialog.h>

class CustomRedimDialog : public QDialog
{
  Q_OBJECT

  QIntValidator * validator;
public:
  Ui_CustomRedimDialog ui;

  CustomRedimDialog(QWidget * parent = nullptr);
};

# endif // CUSTOMREDIMDIALOG_H
