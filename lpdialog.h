#ifndef LPDIALOG_H
#define LPDIALOG_H

#include <QDialog>

namespace Ui {
class LPDialog;
}

class LPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LPDialog(QWidget *parent = 0);
    ~LPDialog();

private:
    Ui::LPDialog *ui;
signals:
    void confirmed(int,double);
public slots:
    void on_buttonBox_accepted();
};

#endif // LPDIALOG_H
