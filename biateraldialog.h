#ifndef BIATERALDIALOG_H
#define BIATERALDIALOG_H

#include <QDialog>

namespace Ui {
class biateralDialog;
}

class biateralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit biateralDialog(QWidget *parent = 0);
    ~biateralDialog();

private:
    Ui::biateralDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(int,double,double);
};

#endif // BIATERALDIALOG_H
