#include "gaussblurdialog.h"
#include "ui_gaussblurdialog.h"

gaussBlurDialog::gaussBlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gaussBlurDialog)
{
    ui->setupUi(this);
}

gaussBlurDialog::~gaussBlurDialog()
{
    delete ui;
}

void gaussBlurDialog::accept()
{
    QString temp=ui->comboBox->currentText();
    bool ok_size;
    int size=ui->sizeEdit->text().toInt(&ok_size);
    bool ok_sigma;
    double sigma=ui->sigmaEdit->text().toDouble(&ok_sigma);
    if(ok_sigma&&ok_size)
    {
        emit confirmed(size,sigma,temp);
    }
    close();
}
