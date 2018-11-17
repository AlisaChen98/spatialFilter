#include "hpdialog.h"
#include "ui_hpdialog.h"

HPDialog::HPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HPDialog)
{
    ui->setupUi(this);
}

HPDialog::~HPDialog()
{
    delete ui;
}

void HPDialog::on_buttonBox_accepted()
{
    bool sizeOK;
    bool sigmaOK;

    int size=ui->sizeEdit->text().toDouble(&sizeOK);
    double sigma=ui->sigmaEdit->text().toDouble(&sigmaOK);
    if(sizeOK&&sigmaOK)
    {
        emit(confirmed(size,sigma));
    }
}
