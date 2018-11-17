#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <mypixmapitem.h>
#include <QImage>
#include <dialoglog.h>
#include <gaussblurdialog.h>
#include <biateraldialog.h>
#include <mediandialog.h>
#include <dilationdialog.h>
#include <erodedialog.h>
#include <affinedialog.h>
#include <matrix.h>
#include <stretchdialog.h>
#include <logpara.h>
#include <lpdialog.h>
#include <hpdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFileDialog *filedialog;
    QGraphicsScene *scene;
    QImage *image;
    //QGraphicsPixmapItem *pixmapItem;
    MyPixmapItem *myPixmapItem;
    QFileInfoList fileList;
    Dialoglog *dlglog;
    gaussBlurDialog *gBdlg;
    biateralDialog *biDlg;
    medianDialog *meDlg;
    dilationDialog *diDlg;
    erodeDialog *erDlg;
    affineDialog *affDlg;
    stretchDialog* strlog;
    logPara* logpara;
    LPDialog* lpDlg;
    HPDialog* hpDlg;

    void open();
    void reset();
    void previous();
    void next();
    void showImage(QImage);
    std::vector<double>parseMatrix1(QString ,int &);
    bool readOneRow(QString ,std::vector<double>& ,int &);
    int currentFileIndex;
    bool getDisplayedImage(QImage&) const;

public slots:
    void on_openButton_clicked();
    void on_resetButton_clicked();
    void on_previousButton_clicked();
    void on_nextButton_clicked();
    void on_Dialoglog_confirmed(QString,QString);
    void on_actionLinear_triggered();
    void on_actionGaussBlur_triggered();
    void on_gaussBlurDialog_confirmed(int,double,QString);
    void on_actionBilateralFilter_triggered();
    void on_biateralDialog_confirmed(int,double,double);
    void on_actionMedianFilter_triggered();
    void on_medianDialog_confirmed(int,QString);
    void on_actionDilation_triggered();
    void on_dilationDialog_confirmed(int,int);
    void on_actionErosion_triggered();
    void on_erodeDialog_confirmed(int, int);
    void on_actionfftTransform2_triggered();
    void on_affineDialog_confirmed(QMatrix);
    void on_actionAffine_triggered();
    void on_stretchDialog_confirmed(double,double);

    void on_actionrgb2GRAY_triggered();
    void on_actionreverse_triggered();
    void on_actionlogTransform_triggered();
    void on_actionContrastStretch_triggered();
    void on_actionhistTransform_triggered();
    void on_actioncolorBalanceOne_triggered();
    void on_actioncolorBalanceTwo_triggered();
    void on_logPara_confirmed(double);
    void on_actionSaveImage_triggered();
    void on_actionLPFilter_triggered();
    void on_actionHPFilter_triggered();
    void on_LPDialog_confirmed(int size,double sigma);
    void on_HPDialog_confirmed(int size,double sigma);
};

#endif // MAINWINDOW_H
