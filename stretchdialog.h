#ifndef STRETCHDIALOG_H
#define STRETCHDIALOG_H

#include <QDialog>

namespace Ui {
class stretchDialog;
}

class stretchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit stretchDialog(QWidget *parent = 0);
    ~stretchDialog();

private:
    Ui::stretchDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(double,double);
};

#endif // STRETCHDIALOG_H
