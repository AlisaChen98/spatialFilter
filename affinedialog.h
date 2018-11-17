#ifndef AFFINEDIALOG_H
#define AFFINEDIALOG_H

#include <QDialog>
#include <matrix.h>

namespace Ui {
class affineDialog;
}

class affineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit affineDialog(QWidget *parent = 0);
    ~affineDialog();
signals:
    void confirmed(QMatrix);
public slots:
    void accept();

private:
    Ui::affineDialog *ui;
};

#endif // AFFINEDIALOG_H
