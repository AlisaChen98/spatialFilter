#ifndef MEDIANDIALOG_H
#define MEDIANDIALOG_H

#include <QDialog>

namespace Ui {
class medianDialog;
}

class medianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit medianDialog(QWidget *parent = 0);
    ~medianDialog();

private:
    Ui::medianDialog *ui;
public slots:
    void accept();
signals:
    void confirmed(int,QString);
};

#endif // MEDIANDIALOG_H
