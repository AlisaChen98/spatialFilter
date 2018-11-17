#include "mediandialog.h"
#include "ui_mediandialog.h"

medianDialog::medianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::medianDialog)
{
    ui->setupUi(this);
}

medianDialog::~medianDialog()
{
    delete ui;
}

void medianDialog::accept()
{
    bool ok;
    int size=ui->lineEdit->text().toInt(&ok);
    QString temp=ui->comboBox->currentText();
    if(ok)
        emit confirmed(size,temp);
    close();
}
