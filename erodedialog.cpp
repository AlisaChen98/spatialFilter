#include "erodedialog.h"
#include "ui_erodedialog.h"

erodeDialog::erodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::erodeDialog)
{
    ui->setupUi(this);
}

erodeDialog::~erodeDialog()
{
    delete ui;
}

void erodeDialog::accept()
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
