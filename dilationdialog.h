#ifndef DILATIONDIALOG_H
#define DILATIONDIALOG_H

#include <QDialog>

namespace Ui {
class dilationDialog;
}

class dilationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dilationDialog(QWidget *parent = 0);
    ~dilationDialog();

private:
    Ui::dilationDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(int,int);
};

#endif // DILATIONDIALOG_H
