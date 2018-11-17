#include "affinedialog.h"
#include "ui_affinedialog.h"

affineDialog::affineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::affineDialog)
{
    ui->setupUi(this);
}

affineDialog::~affineDialog()
{
    delete ui;
}

//赋值给矩阵
void affineDialog::accept()
{
    bool flag[4];
    double x00=ui->lineEdit->text().toDouble(&flag[0]);
    double x01=ui->lineEdit_2->text().toDouble(&flag[1]);
    double x10=ui->lineEdit_4->text().toDouble(&flag[2]);
    double x11=ui->lineEdit_5->text().toDouble(&flag[3]);
    double x20=ui->lineEdit_7->text().toDouble(&flag[4]);
    double x21=ui->lineEdit_8->text().toDouble(&flag[5]);
    if(flag[0]&&flag[1]&&flag[2]&&flag[3]&&flag[4]&&flag[5])
    {
        QMatrix m;
        m.setMatrix(x00,x01,x10,x11,x20,x21);
        emit(confirmed(m));
    }
    close();
}
