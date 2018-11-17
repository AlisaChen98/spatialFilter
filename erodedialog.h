#ifndef ERODEDIALOG_H
#define ERODEDIALOG_H

#include <QDialog>

namespace Ui {
class erodeDialog;
}

class erodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit erodeDialog(QWidget *parent = 0);
    ~erodeDialog();

private:
    Ui::erodeDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(int,int);
};

#endif // ERODEDIALOG_H
