#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QImage>
#include <QMatrix>
#include <iostream>
#include <matrix.h>

struct HSI
{
    double H;
    double S;
    double I;
};


class imageprocessor
{
public:
    imageprocessor();

    static QImage rgb2gray(const QImage&);

    //图像反转
    static QImage reverse(const QImage&);

    //对数变换的参数如何传递
    static QImage logTransformation(const QImage&,double);

    //对比度拉伸
    static QImage contrastStretch(const QImage&,double,double);

    static QImage hist(const QImage&);

    static QImage colorOne(const QImage&);

    static int * cal(int *,int,int);

    static QImage colorTwo(const QImage&);

    static void cumulateHist(int *,int *,int);

    static HSI rgb2hsi(QRgb);

    static QRgb hsi2rgb(QRgb,HSI &);

    static double hue2rgb(double,double,double);

    //线性滤波
    static QImage linearFilter(const QImage&,std::vector<double>,int);

    //补零
    static QImage zeroPadding(const QImage&,int,int);

    static QImage filter(const QImage&,std::vector<double>,int,int);

    static std::vector<int> getPatch(const QImage&,int,int,int,int,char);

    static QImage crop(const QImage&,int,int);

    static void kernelNormalization(std::vector<double>&);

    static QImage gaussBlur(const QImage&,int,double);

    static std::vector<double> createGaussKernel(int size,double sigma);

    static QImage bilateralFilter(const QImage&,int ,double,double);

    static std::vector<double> createBilateralColorKernel(double);

    static QImage medianFilter(const QImage&,int);

    static QImage dilationFilter(const QImage&,int,int);

    static QImage erosionFilter(const QImage&,int,int);

    //affineTransform
    static QImage affineTransform(const QImage&,QMatrix);
    static QRgb bilinearInput(double x,double y,const QImage &img);

    //低通和高通滤波
    static QImage gaussBlurLP(const QImage &, const int , const double );
    static QImage gaussBlurHP(const QImage &, const int , const double );
};

#endif // IMAGEPROCESSOR_H
