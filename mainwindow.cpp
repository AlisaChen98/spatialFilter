#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <imageprocessor.h>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <matrix.h>
#include <fft.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    filedialog(new QFileDialog(this)),
    scene(NULL),
    myPixmapItem(NULL),
    currentFileIndex(-1),
    dlglog(new Dialoglog(this)),
    gBdlg(new gaussBlurDialog(this)),
    biDlg(new biateralDialog(this)),
    meDlg(new medianDialog(this)),
    diDlg(new dilationDialog(this)),
    erDlg(new erodeDialog(this)),
    affDlg(new affineDialog(this)),
    logpara(new logPara(this)),
    strlog(new stretchDialog(this)),
    lpDlg(new LPDialog(this)),
    hpDlg(new HPDialog(this))

{
    //filedialog=new QFileDialog(this);
    filedialog->setNameFilter("Image(*.jpg *.bmp *.png *.gif)");//设置文件后缀名过滤器
    filedialog->setFileMode(QFileDialog::ExistingFile);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete filedialog;
    delete myPixmapItem;
    delete scene;
    delete dlglog;
    delete gBdlg;
    delete biDlg;
    delete meDlg;
    delete diDlg;
    delete erDlg;
    delete affDlg;
    delete logpara;
    delete strlog;
    delete lpDlg;
    delete hpDlg;
}

void MainWindow::on_openButton_clicked()
{
    open();
}

void MainWindow::on_resetButton_clicked()
{
    reset();
}

void MainWindow::on_previousButton_clicked()
{
    previous();
}

void MainWindow::on_nextButton_clicked()
{
    next();
}

bool MainWindow::readOneRow(QString row,std::vector<double>& kernel,int &nCol)
{
    nCol=0;
    QStringList cols=row.trimmed().split(QRegularExpression("\\s*,|\\s+"));
    for(int j=0;j<cols.size();++j)
    {
        bool ok;
        QString col=cols[j];
        if(col.trimmed().isEmpty())continue;
        double d=col.trimmed().toDouble(&ok);
        if(ok)
        {
            nCol++;
            kernel.push_back(d);
        }
        else return false;
    }
    return true;
}

//匹配分隔符的正则表达式
std::vector<double> MainWindow::parseMatrix1(QString txt,int &nCol)//返回vector保存所有数字
{
    //replace是去掉换行，trimmed函数是去掉前后的空格，split根据分号进行切割
    QStringList rows=txt.replace("\n","").trimmed().split(QRegularExpression("\\s*;"));
    nCol=-1;//表示矩阵的列数
    std::vector<double> ret;
    //循环每次处理一行
    for(int i=0;i<rows.size();++i)
    {
        bool ok;
        QString row=rows[i];
        if(row.trimmed().isEmpty())continue;//跳过空行
        if(nCol==-1)//表示现在是第一行
        {
            int temp;
            ok=readOneRow(row,ret,temp);
            if(ok)
                nCol=temp;
            else
                return std::vector<double>();
        }
        else//表示是其他行
        {
            int temp;
            ok=readOneRow(row,ret,temp);
             if(ok&&temp==nCol)
                continue;
            else
            {
                nCol=-1;
                return std::vector<double>();
            }
        }
    }
    return ret;
}


void MainWindow::on_actionLinear_triggered()
{
    dlglog->exec();
}

void MainWindow::on_actionGaussBlur_triggered()
{
    gBdlg->exec();
}

void MainWindow::on_actionBilateralFilter_triggered()
{
    biDlg->exec();
}

void MainWindow::on_actionMedianFilter_triggered()
{
    meDlg->exec();
}

void MainWindow::on_actionDilation_triggered()
{
    diDlg->exec();
}

void MainWindow::on_actionErosion_triggered()
{
    erDlg->exec();
}

void MainWindow::on_gaussBlurDialog_confirmed(int size,double sigma,QString temp)
{
    QImage img;
    bool ok=getDisplayedImage(img);
    QString s="Gray";
    qDebug()<<temp;
    if(ok)
    {
        if(!(temp.compare(s)))
        {
            qDebug()<<temp.compare(s);
            img=imageprocessor::rgb2gray(img);
        }
        showImage(imageprocessor::gaussBlur(img,size,sigma));
    }
}

void MainWindow::on_biateralDialog_confirmed(int size,double color,double space)
{
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok)
        showImage(imageprocessor::bilateralFilter(img,size,color,space));
}

void MainWindow::on_medianDialog_confirmed(int size,QString temp)
{
    QImage img;
    bool ok=getDisplayedImage(img);
    QString s="Gray";
    qDebug()<<temp;
    if(ok)
    {
        if(!(temp.compare(s)))
        {
            qDebug()<<temp.compare(s);
            img=imageprocessor::rgb2gray(img);
        }
        showImage(imageprocessor::medianFilter(img,size));
    }
}

void MainWindow::on_dilationDialog_confirmed(int width,int height)
{
    QImage img;
    bool ok=getDisplayedImage(img);
//    QString s="Gray";
//    qDebug()<<temp;
    if(ok)
    {
//        if(!(temp.compare(s)))
//        {
//            qDebug()<<temp.compare(s);
//            img=imageprocessor::rgb2gray(img);
//        }
        showImage(imageprocessor::dilationFilter(img,width,height));
    }
}

void MainWindow::on_erodeDialog_confirmed(int width,int height)
{
    QImage img;
    bool ok=getDisplayedImage(img);
//    QString s="Gray";
//    qDebug()<<temp;
    if(ok)
    {
//        if(!(temp.compare(s)))
//        {
//            qDebug()<<temp.compare(s);
//            img=imageprocessor::rgb2gray(img);
//        }
        showImage(imageprocessor::erosionFilter(img,width,height));
    }
}

void MainWindow::on_actionfftTransform2_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {
        Matrix<int> temp=Matrix<int>::fromQImage(img,'G');
//        cout << temp << endl;
        Matrix<std::complex<double>> fftDimension=fft2d(temp,temp.getNRow(),temp.getNCol());
//        cout<<"fftDimension "<<fftDimension<<endl;
//        Matrix<std::complex<double>> fftDimension=fft2d(temp,temp.getNRow(),temp.getNCol());
        Matrix<double> fftABS=Matrix<double>::abs2stats(fftDimension);
        Matrix<double> fftlog=Matrix<double>::logTransformation(fftABS,1);
        Matrix<double>::fftshift(fftlog);
        Matrix<int> fftRes=Matrix<int>::normalization(fftlog);
        QImage res=Matrix<int>::toQImage(fftRes);
        qDebug()<<res.width()<<" "<<res.height();
        showImage(res);
        if(myPixmapItem)
        {
            myPixmapItem->reset();
            QRectF rect=myPixmapItem->boundingRect();
            scene->setSceneRect(0,0,rect.width(),rect.height());
            ui->graphicsView->fitInView(rect,Qt::KeepAspectRatio);
            ui->graphicsView->centerOn(myPixmapItem);
        }
        showImage(res);
    }
}

void MainWindow::on_affineDialog_confirmed(QMatrix mat)
{
    QImage img;
    if(getDisplayedImage(img))
    {
        showImage(imageprocessor::affineTransform(img,mat));
    }
//    if(myPixmapItem)
//    {
//        myPixmapItem->reset();
//        QRectF rect=myPixmapItem->boundingRect();
//        scene->setSceneRect(0,0,rect.width(),rect.height());
//        ui->graphicsView->fitInView(rect,Qt::KeepAspectRatio);
//        ui->graphicsView->centerOn(myPixmapItem);
//    }
//    showImage(imageprocessor::affineTransform(img,mat));
}

void MainWindow::on_actionAffine_triggered()
{
    affDlg->exec();
}

void MainWindow::open()
{
    if(filedialog->exec())
    {
     QStringList files=filedialog->selectedFiles();
     QDir dir=filedialog->directory();
     QStringList filters;
     filters<<"*.bmp"<<"*.jpg"<<"*.png";
     fileList=dir.entryInfoList(filters);
     for(int i=0;i<fileList.size();++i)
     {
         if(fileList[i].absoluteFilePath()==files[0])
         {
             currentFileIndex=i;
             break;
         }
     }
     qDebug()<<files[0]<<"\t"<<currentFileIndex;
     showImage(QImage(fileList[currentFileIndex].absoluteFilePath()));
     reset();
    }
}

void MainWindow::reset()
{
    myPixmapItem->reset();
    QRectF rect=myPixmapItem->boundingRect();
    scene->setSceneRect(0,0,rect.width(),rect.height());
    ui->graphicsView->fitInView(rect,Qt::KeepAspectRatio);
    showImage(QImage(fileList[currentFileIndex].absoluteFilePath()));
}

void MainWindow::previous()
{
    if(currentFileIndex==0)
    {
        currentFileIndex=fileList.size();
    }
    showImage(QImage(fileList[--currentFileIndex].absoluteFilePath()));
    reset();
}

void MainWindow::next()
{
    if(currentFileIndex+1<fileList.size())
        currentFileIndex++;
    else currentFileIndex=0;
    showImage(QImage(fileList[currentFileIndex].absoluteFilePath()));
    reset();
}

void MainWindow::showImage(QImage image)
{
    if(myPixmapItem)
    {
        delete myPixmapItem;
        myPixmapItem=NULL;
    }
    if(scene)
    {
        delete scene;
        scene=NULL;
    }
    myPixmapItem=new MyPixmapItem();
    myPixmapItem->setPixmap(QPixmap::fromImage(image));
    scene=new QGraphicsScene(this);
    scene->addItem(myPixmapItem);
    ui->graphicsView->setScene(scene);
}

bool MainWindow::getDisplayedImage(QImage &img) const
{
    QList<QGraphicsItem *> items=ui->graphicsView->scene()->items();//获得场景中的所有物品
    //需要判断是否打开文件
    if(items.size()==0)return false;
    QGraphicsPixmapItem* item=(QGraphicsPixmapItem*)items.at(0);
    img=item->pixmap().toImage();
    return true;
}

void MainWindow::on_Dialoglog_confirmed(QString txt,QString temp)
{
    qDebug()<<123456;
    int nCol;
    std::vector<double> kernel = parseMatrix1(txt,nCol);
    qDebug()<<txt;
    imageprocessor::kernelNormalization(kernel);
    if(-1==nCol) return;
    QImage img;
    QString s="Gray";
    qDebug()<<temp;
    if(getDisplayedImage(img))
    {
        if(!(temp.compare(s)))
        {
            qDebug()<<temp.compare(s);
            img=imageprocessor::rgb2gray(img);
        }
        QImage res=imageprocessor::linearFilter(img,kernel,nCol);
        showImage(res);
    }

}

void MainWindow::on_stretchDialog_confirmed(double m,double e)
{
    qDebug()<<54321;
    QImage img;
    if(getDisplayedImage(img))
    {
        QImage res=imageprocessor::contrastStretch(img,m,e);
//        qDebug()<<c;
        showImage(res);
    }
}

void MainWindow::on_actionrgb2GRAY_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {
        Matrix<int> temp=Matrix<int>::fromQImage(img,'G');
        QImage res=Matrix<int>::toQImage(temp);
        qDebug()<<res.width()<<" "<<res.height();
        showImage(res);

     //rgb2gray,界面和算法不要混在一起
//     QImage res=imageprocessor::rgb2gray(img);
//     showImage(res);
    }
}

void MainWindow::on_actionreverse_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {

        QImage res=imageprocessor::reverse(img);
        showImage(res);
    }
}

void MainWindow::on_actionhistTransform_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {
        QImage res=imageprocessor::hist(img);
        showImage(res);
    }
}

void MainWindow::on_actioncolorBalanceOne_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {
        QImage res=imageprocessor::colorOne(img);
        showImage(res);
    }
}

void MainWindow::on_actioncolorBalanceTwo_triggered()
{
    QImage img;
    if(getDisplayedImage(img))
    {
        QImage res=imageprocessor::colorTwo(img);
        showImage(res);
    }
}

void MainWindow::on_actionlogTransform_triggered()
{
    logpara->exec();
}

void MainWindow::on_actionContrastStretch_triggered()
{
    strlog->exec();
}

void MainWindow::on_logPara_confirmed(double c)
{
    qDebug()<<12345;
    QImage img;
    if(getDisplayedImage(img))
    {
        QImage res=imageprocessor::logTransformation(img,c);
        qDebug()<<c;
        showImage(res);
    }
}

void MainWindow::on_actionSaveImage_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("*.png;; *.jpg;; *.bmp;; *.gif;;"));
        if(filename.isEmpty()){
            return;
        }else{
            if(!(img.save(filename))){
                return;
            }
        }
    }
}

void MainWindow::on_LPDialog_confirmed(int size,double sigma)
{
    QImage img;
    if(getDisplayedImage(img))
    {
        showImage(imageprocessor::gaussBlurLP(img,size,sigma));
    }
}

void MainWindow::on_actionLPFilter_triggered()
{
    lpDlg->exec();
}

void MainWindow::on_HPDialog_confirmed(int size,double sigma)
{
    QImage img;
    if(getDisplayedImage(img))
    {
        showImage(imageprocessor::gaussBlurHP(img,size,sigma));
    }
}

void MainWindow::on_actionHPFilter_triggered()
{
    hpDlg->exec();
}
