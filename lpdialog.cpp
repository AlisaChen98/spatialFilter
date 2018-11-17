#include "lpdialog.h"
#include "ui_lpdialog.h"

LPDialog::LPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LPDialog)
{
    ui->setupUi(this);
}

LPDialog::~LPDialog()
{
    delete ui;
}

void LPDialog::on_buttonBox_accepted()
{
    bool sizeOK;
    bool sigmaOK;

    int size=ui->editSize->text().toDouble(&sizeOK);
    double sigma=ui->editSigma->text().toDouble(&sigmaOK);
    if(sizeOK&&sigmaOK)
    {
        emit(confirmed(size,sigma));
    }
}
