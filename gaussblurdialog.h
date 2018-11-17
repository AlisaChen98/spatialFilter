#ifndef GAUSSBLURDIALOG_H
#define GAUSSBLURDIALOG_H

#include <QDialog>

namespace Ui {
class gaussBlurDialog;
}

class gaussBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit gaussBlurDialog(QWidget *parent = 0);
    ~gaussBlurDialog();

private:
    Ui::gaussBlurDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(int,double,QString);
};

#endif // GAUSSBLURDIALOG_H
