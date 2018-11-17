#include "biateraldialog.h"
#include "ui_biateraldialog.h"

biateralDialog::biateralDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::biateralDialog)
{
    ui->setupUi(this);
}

biateralDialog::~biateralDialog()
{
    delete ui;
}

void biateralDialog::accept()
{
    bool ok_size;
    int size=ui->sizeEdit->text().toInt(&ok_size);
    bool ok_color;
    double color=ui->colorEdit->text().toDouble(&ok_color);
    bool ok_space;
    double space=ui->spaceEdit->text().toDouble(&ok_space);
    if(ok_size&&ok_color&&ok_space)
        emit confirmed(size,color,space);
    close();
}
