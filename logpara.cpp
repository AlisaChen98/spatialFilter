#include "logpara.h"
#include "ui_logpara.h"

logPara::logPara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logPara)
{
    ui->setupUi(this);
}

logPara::~logPara()
{
    delete ui;
}

void logPara::accept()
{
    bool ok;
    double c;
    c=ui->lineEdit->text().toDouble(&ok);//把空间中的文本转成double
    if(ok)
    {
        //emit发送信号
        emit confirmed(c);
    }
    close();
}
