#ifndef LOGPARA_H
#define LOGPARA_H

#include <QDialog>

namespace Ui {
class logPara;
}

class logPara : public QDialog
{
    Q_OBJECT

public:
    explicit logPara(QWidget *parent = 0);
    ~logPara();

private:
    Ui::logPara *ui;
public slots:
    void accept();//需要重写这个自带的函数
signals:
    void confirmed(double);
};

#endif // LOGPARA_H
