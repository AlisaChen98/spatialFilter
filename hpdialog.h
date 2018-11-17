#ifndef HPDIALOG_H
#define HPDIALOG_H

#include <QDialog>

namespace Ui {
class HPDialog;
}

class HPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HPDialog(QWidget *parent = 0);
    ~HPDialog();

private:
    Ui::HPDialog *ui;
signals:
    void confirmed(int,double);
public slots:
    void on_buttonBox_accepted();
};

#endif // HPDIALOG_H
