#include "dilationdialog.h"
#include "ui_dilationdialog.h"

dilationDialog::dilationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dilationDialog)
{
    ui->setupUi(this);
}

dilationDialog::~dilationDialog()
{
    delete ui;
}

void dilationDialog::accept()
{
//    QString temp=ui->comboBox->currentText();
    bool ok_width;
    int width=ui->widthEdit->text().toInt(&ok_width);
    bool ok_height;
    int height=ui->heightEdit->text().toInt(&ok_height);
    if(ok_width&&ok_height)
    {
        emit confirmed(width,height);
    }
    close();
}
