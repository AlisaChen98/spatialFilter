#include "stretchdialog.h"
#include "ui_stretchdialog.h"
#include <qdebug.h>
#include <QObject>

stretchDialog::stretchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stretchDialog)
{
    ui->setupUi(this);
}

stretchDialog::~stretchDialog()
{
    delete ui;
}

void stretchDialog::accept()
{
    bool okm;
    double m;
    m=ui->lineEdit->text().toDouble(&okm);
    bool oke;
    double e;
    e=ui->lineEdit_2->text().toDouble(&oke);
    qDebug()<<m<<" "<<e;
    if(oke&&okm)
    {
        emit confirmed(m,e);
//        emit confirmed(e);
        qDebug()<<"already used";
    }
    close();
}


