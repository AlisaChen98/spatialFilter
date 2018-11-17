#include "dialoglog.h"
#include "ui_dialoglog.h"

Dialoglog::Dialoglog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoglog)
{
    ui->setupUi(this);
}

Dialoglog::~Dialoglog()
{
    delete ui;
}

void Dialoglog::accept()
{
    QString c=ui->textEdit->toPlainText();
    QString temp=ui->comboBox->currentText();
    emit confirmed(c,temp);
    close();
}
