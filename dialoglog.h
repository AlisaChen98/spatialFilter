#ifndef DIALOGLOG_H
#define DIALOGLOG_H

#include <QDialog>

namespace Ui {
class Dialoglog;
}

class Dialoglog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoglog(QWidget *parent = 0);
    ~Dialoglog();

private:
    Ui::Dialoglog *ui;
public slots:
    void accept();
signals:
    void confirmed(QString,QString);
};

#endif // DIALOGLOG_H
