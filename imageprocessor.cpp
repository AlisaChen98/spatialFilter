#include "imageprocessor.h"
#include <QImage>
#include <iostream>
#include <cmath>
#include <QDebug>
#include <matrix.h>
#include <fft.h>

imageprocessor::imageprocessor()
{

}

QImage imageprocessor::rgb2gray(const QImage &img)
{

    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    int w=ret.width();
    int h=ret.height();
    //先宽后高，以免越界
    for(int i=0;i<w;++i)
    {
        for(int j=0;j<h;++j)
        {
            QRgb rgb=ret.pixel(i,j);
            //读取像素颜色信息
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            int gray=(r+g+b)/3;
            ret.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return ret;
}

QImage imageprocessor::reverse(const QImage &img)
{
    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    Matrix<int> tempR=Matrix<int>::fromQImage(img,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(img,'b');
    tempR=255-tempR;
    tempG=255-tempG;
    tempB=255-tempB;
    ret=Matrix<int>::toQImage(tempR,tempG,tempB);
    return ret;
}

QImage imageprocessor::logTransformation(const QImage &img,double c)
{
    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'G');
    int w=tempG.getNCol();
    int h=tempG.getNRow();
    for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
      {
          double r=tempG(i,j)/255.0;
          int res=c*log(1+r)*255;
          if(res>255)res=255;
          if(res<0)res=0;
          tempG(i,j)=res;
      }
      qDebug()<<19980214<<endl;
    }
    ret=Matrix<int>::toQImage(tempG);
    return ret;
}

QImage imageprocessor::contrastStretch(const QImage &img,double m,double e)
{
    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    int w=ret.width();
    int h=ret.height();
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'G');
        for(int i=0;i<w;++i)
        {
            for(int j=0;j<h;++j)
            {
                double r=tempG(j,i);//表示浮点数
                double a=m/r;
                double res=1/(1+pow(a,e))*255;
                if(res>255)res=255;
                if(res<0)res=0;
                tempG(j,i)=res;
            }
        }
            ret=Matrix<int>::toQImage(tempG);
    return ret;
}

QImage imageprocessor::hist(const QImage &img)
{
    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    int w=ret.width();
    int h=ret.height();
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'G');
    int gray[256]={0};//记录每个灰度级别下的像素个数
    double gray_pro[256]={0};//记录灰度分布密度
    double gray_dis[256]={0};//记录累计密度
    int gray_eql[256]={0};//记录均衡化后的灰度值
    int gray_sum=w*h;
    //先宽后高，以免越界
    //统计每个灰度下的像素个数
    for (int i=0;i<w;++i)
    {
        for(int j=0;j<h;++j)
        {
            gray[tempG(j,i)]++;
        }
    }
    //统计灰度频率
    for (int i=0;i<256;++i)
    {
        gray_pro[i]=((double)gray[i]/gray_sum);
    }
    //计算累计密度
    gray_dis[0]=gray_pro[0];
    for(int i=1;i<256;i++)
    {
        gray_dis[i]=gray_dis[i-1]+gray_pro[i];
    }
    for(int i=0;i<256;i++)
    {
        gray_eql[i]=(uchar)(255*gray_dis[i]+0.5);
    }
    for (int i=0;i<w;++i)
    {
        for(int j=0;j<h;++j)
        {
            int g=gray_eql[tempG(j,i)];
            tempG(j,i)=g;
        }
    }
    ret=Matrix<int>::toQImage(tempG);
    return ret;
}

QImage imageprocessor::colorOne(const QImage &img)
{
    QImage ret(img);//用于返回的变量,img是常量不能修改故需要复制
    int width=ret.width();
    int height=ret.height();
    int s=width*height;

    int hist[3][256],map[3][256];
    std::fill(hist[0],hist[0]+3*256,0);
    Matrix<int> tempR=Matrix<int>::fromQImage(img,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(img,'b');
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
      {
            hist[0][tempR(j,i)]++;
            hist[1][tempG(j,i)]++;
            hist[2][tempB(j,i)]++;
        }
    }
    cumulateHist(hist[0],map[0],s);
    cumulateHist(hist[1],map[1],s);
    cumulateHist(hist[2],map[2],s);

    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            int r=map[0][tempR(j,i)];
            int g=map[1][tempG(j,i)];
            int b=map[2][tempB(j,i)];
            tempR(j,i)=r;
            tempG(j,i)=g;
            tempB(j,i)=b;
        }
    }
    ret=Matrix<int>::toQImage(tempR,tempG,tempB);
    return ret;
}


void imageprocessor::cumulateHist(int *hist, int *map,int N)
{
    double sum=0;
    for(int i=0;i<256;++i)
    {
        sum+=hist[i];
        map[i]=round(sum/N*255);
    }
}

QImage imageprocessor::colorTwo(const QImage &img)
{
    QImage ret(img);
    int width=ret.width();
    int height=ret.height();
    long N=width*height;
    //RGBTOTHSI
    HSI *Hsi=new HSI[N];
    Matrix<int> tempR=Matrix<int>::fromQImage(img,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(img,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(img,'b');
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            QRgb rgb=qRgb(tempR(j,i),tempG(j,i),tempB(j,i));
            Hsi[i*height+j]=HSI();
//            HSI temp1;
            Hsi[i*height+j]=rgb2hsi(rgb);
//            Hsi[i*height+j].I=temp1.I;
//            Hsi[i*height+j].H=temp1.H;
//            Hsi[i*height+j].S=temp1.S;
            //qDebug()<<Hsi[height*i+j].I<<endl;
        }
    }
    //计算I的直方图
    int hist[256];
    std::fill(hist,hist+256,0);
    for(int i=0;i<N;i++)
        hist[int((Hsi[i].I)*255)]++;
    int map[256];
    cumulateHist(hist,map,N);
    for(int i=0;i<N;++i)
        Hsi[i].I=map[int((Hsi[i].I)*255)]/255.;
    //HSITORGB
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            QRgb rgb,rgb1;
            rgb1=hsi2rgb(rgb,Hsi[i*height+j]);
            tempR(j,i)=qRed(rgb1);
            tempG(j,i)=qGreen(rgb1);
            tempB(j,i)=qBlue(rgb1);
        }
    }
    delete Hsi;
    ret=Matrix<int>::toQImage(tempR,tempG,tempB);
    return ret;
}

HSI imageprocessor::rgb2hsi(QRgb rgb)
{
    //取得RGB三个通道的分量值
    int R=qRed(rgb);
    int G=qGreen(rgb);
    int B=qBlue(rgb);
    double H,S,I;
    //qDebug()<<R<<" "<<G<<" "<<B<<" ";

              //计算归一化的RGB值
    double var_R=R/255.;
    double var_G=G/255.;
    double var_B=B/255.;

    //qDebug()<<var_R;

    //找最小值
    double var_min=var_R;
    if(var_G<=var_min) var_min=var_G;
    if(var_B<=var_min) var_min=var_B;
    //找最大值
    double var_max=var_R;
    if(var_G>=var_max) var_max=var_G;
    if(var_B>=var_max) var_max=var_B;
    double del_R=0;
    double del_G=0;
    double del_B=0;
    double del_max=var_max-var_min;
    I=(var_max+var_min)/2.;

    if(del_max==0)
    {
        H=0;
        S=0;
    }
    else
    {
        if(I<0.5)
            S=del_max/(var_max+var_min);
        else
            S=del_max/(2-var_max-var_min);
        del_R=(((var_max-var_R)/6.)+(del_max/2.))/del_max;
        del_G=(((var_max-var_G)/6.)+(del_max/2.))/del_max;
        del_B=(((var_max-var_B)/6.)+(del_max/2.))/del_max;

        if(var_R==var_max)
            H=del_B-del_G;
        else if(var_G==var_max)
            H=1./3.+del_R-del_B;
        else if(var_B==var_max)
            H=2./3.+del_G-del_R;
        if(H<0)
            H+=1;
        if(H>1)
            H-=1;
    }
    HSI temp;
    temp.H=H;
    temp.S=S;
    temp.I=I;
    return temp;
}

QRgb imageprocessor::hsi2rgb(QRgb rgb, HSI &hsi)
{
    int R=qRed(rgb);
    int G=qGreen(rgb);
    int B=qBlue(rgb);
    double var_1=0;
    double var_2=0;
    if(hsi.S==0)
    {
        R=hsi.I*255;
        G=hsi.I*255;
        B=hsi.I*255;
    }
    else
    {
        if(hsi.I<0.5)
            var_2=hsi.I*(1+hsi.S);
        else
            var_2=(hsi.I+hsi.S)-(hsi.I*hsi.S);

        var_1=2*hsi.I-var_2;

        R=255*hue2rgb(var_1,var_2,hsi.H+1/3.);
        G=255*hue2rgb(var_1,var_2,hsi.H);
        B=255*hue2rgb(var_1,var_2,hsi.H-1/3.);
    }
    QRgb rgb1=qRgb(R,G,B);
    return rgb1;
}

double imageprocessor::hue2rgb(double v1,double v2,double vH)
{
       if ( vH < 0 ) vH += 1;
       if ( vH > 1 ) vH -= 1;
       if ( ( 6 * vH ) < 1 ) return ( v1 + ( v2 - v1 ) * 6 * vH );
       if ( ( 2 * vH ) < 1 ) return ( v2 );
       if ( ( 3 * vH ) < 2 ) return ( v1 + ( v2 - v1 ) * ( ( 2. / 3. ) - vH ) * 6 );
       return ( v1 );
}

QImage imageprocessor::linearFilter(const QImage& img,std::vector<double> kernel,int nCol)
{
    int nRow=kernel.size()/nCol;
    if(0==nRow%2||0==nCol%2)return img;
    //padding
    QImage padded=zeroPadding(img,nCol,nRow);
    //return padded;
    QImage filtered=filter(padded,kernel,nCol,nRow);
    //return filtered;
    QImage croped=crop(filtered,nCol,nRow);
    return croped;
}

QImage imageprocessor::zeroPadding(const QImage& img,int nCol,int nRow)
{
    QImage ret(img.width()+nCol-1,img.height()+nRow-1,img.format());//滤波器大小
    int width=ret.width();
    int height=ret.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(i<nCol/2||i>=width-nCol/2||j<nRow/2||j>=height-nRow/2)
                ret.setPixel(i,j,qRgb(0,0,0));
            else
            {
                QRgb rgb=img.pixel(i-nCol/2,j-nRow/2);
                //先做灰度图
                int r=qRed(rgb);
                int g=qGreen(rgb);
                int b=qBlue(rgb);
                ret.setPixel(i,j,qRgb(r,g,b));
            }
        }
    }
    return ret;
}

QImage imageprocessor::filter(const QImage& img,std::vector<double> kernel,int nCol,int nRow)
{
    QImage ret(img);
    Matrix<int> tempR = Matrix<int>::fromQImage(img, 'r');
    Matrix<int> tempG = Matrix<int>::fromQImage(img, 'g');
    Matrix<int> tempB = Matrix<int>::fromQImage(img, 'b');
    int width=img.width();
    int height=img.height();
    qDebug()<<nRow<<" Fine"<<endl;
    Matrix<double> operation=Matrix<double>::fromKernel(kernel,nCol,nRow);
    for(int i=nCol/2;i<width-nCol/2;i++)
    {
        for(int j=nRow/2;j<height-nRow/2;j++)
        {
            Matrix<int> patchR=tempR.subMatrix(j-nRow/2,j+nRow/2,i-nCol/2,i+nCol/2);
            Matrix<int> patchG=tempG.subMatrix(j-nRow/2,j+nRow/2,i-nCol/2,i+nCol/2);
            Matrix<int> patchB=tempB.subMatrix(j-nRow/2,j+nRow/2,i-nCol/2,i+nCol/2);
            Matrix<double> resR = Matrix<double>::multiplication(patchR, operation);
            Matrix<double> resG = Matrix<double>::multiplication(patchG, operation);
            Matrix<double> resB = Matrix<double>::multiplication(patchB, operation);
            double sumR=0,sumG=0,sumB=0;
            for(int k=0;k<resR.getNCol();k++)
            {
                for(int s=0;s<resR.getNRow();s++)
                    sumR+=resR(s,k);
            }
            for(int k=0;k<resG.getNCol();k++)
            {
                for(int s=0;s<resG.getNRow();s++)
                    sumG+=resG(s,k);
            }
            for(int k=0;k<resB.getNCol();k++)
            {
                for(int s=0;s<resB.getNRow();s++)
                    sumB+=resB(s,k);
            }
            int r=(int)sumR;
            int g=(int)sumG;
            int b=(int)sumB;
            if(r>255)
                r=255;
            if(g>255)
                g=255;
            if(b>255)
                b=255;
            if(r<0)
                r=0;
            if(g<0)
                g=0;
            if(b<0)
                b=0;
            tempR(j,i)=r;
            tempG(j,i)=g;
            tempB(j,i)=b;
        }
    }
    ret=Matrix<int>::toQImage(tempR,tempG,tempB);
    return ret;
}

std::vector<int> imageprocessor::getPatch(const QImage& img,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for(int i=w-nCol/2;i<=w+nCol/2;i++)
    {
        for(int j=h-nRow/2;j<=h+nRow/2;j++)
        {
            switch(c)
            {
            case 'r':
                ret.push_back(qRed(img.pixel(i,j)));
                break;
            case 'g':
                ret.push_back(qGreen(img.pixel(i,j)));
                break;
            case 'b':
                ret.push_back(qBlue(img.pixel(i,j)));
                break;
            case 'y':
                ret.push_back(qGray(img.pixel(i,j)));
                break;
            default:
                break;
            }
        }
    }
    return ret;
}

QImage imageprocessor::crop(const QImage &img,int nCol,int nRow)
{
    QImage ret(img.width()-nCol+1,img.height()-nRow+1,img.format());
    int width=ret.width();
    int height=ret.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            ret.setPixel(i,j,img.pixel(i+nCol/2,j+nRow/2));
        }
    }
    return ret;
}

void imageprocessor::kernelNormalization(std::vector<double>& kernel)
{
    double sum=0;
    for(int i=0;i<kernel.size();i++)
    sum+=kernel[i];
    if(0!=sum)
    {
        for(int i=0;i<kernel.size();i++)
            kernel[i]/=sum;
    }
}

QImage imageprocessor::gaussBlur(const QImage& img,int size,double sigma)
{
    std::vector<double> kernel=createGaussKernel(size,sigma);
    kernelNormalization(kernel);
    return linearFilter(img,kernel,size);
}

std::vector<double> imageprocessor::createGaussKernel(int size,double sigma)
{
    std::vector<double> ret;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            int deltaX=abs(i-size/2);
            int deltaY=abs(j-size/2);
            ret.push_back(exp((-deltaX*deltaX-deltaY*deltaY)/(2*sigma*sigma)));
        }
    }
    return ret;
}

QImage imageprocessor::bilateralFilter(const QImage& img,int size,double color,double space)
{        
    //padding
    QImage padded=zeroPadding(img,size,size);
    int width=padded.width();
    int height=padded.height();
    //Normalization
    std::vector<double> kernelSpace=createGaussKernel(size,space);
    std::vector<double> kernelColor=createBilateralColorKernel(color);
    Matrix<double> operationSpace=Matrix<double>::fromKernel(kernelSpace,size,size);
    //Filter
    Matrix<int> filtered=Matrix<int>::fromQImage(padded,'G');
    Matrix<int> PAD=Matrix<int>::fromQImage(padded,'G');
    for(int i=size/2;i<width-size/2;i++)
    {
        for(int j=size/2;j<height-size/2;j++)
        {
            Matrix<int> patch=PAD.subMatrix(j-size/2,j+size/2,i-size/2,i+size/2);
            Matrix<double> operation(size,size,0);
            for(int k=0;k<size;k++)
            {
                for(int s=0;s<size;s++)
                {
                    int delta=abs(filtered(k,s)-PAD(j,i));
                    operation(k,s)=kernelColor[delta]*operationSpace(k,s);
                }
            }
            Matrix<double>::kernelCal(operation);
            double sum=0;
            for(int k=0;k<size;k++)
            {
                for(int s=0;s<size;s++)
                {
                    sum+=operation(k,s)*patch(k,s);
                }
            }
//            qDebug()<<sum;
            int y=(int)sum;
            if(y>255)
                y=255;
            filtered(j,i)=y;
        }
    }
    QImage Filtered=Matrix<int>::toQImage(filtered);
    QImage cropped=crop(Filtered,size,size);
    return cropped;
}

std::vector<double> imageprocessor::createBilateralColorKernel(double sigmacolor)
{
    std::vector<double> ret;
    for(int i=0;i<256;i++)
        ret.push_back(exp((-i*i)/(2*sigmacolor*sigmacolor)));
    return ret;
}

QImage imageprocessor::medianFilter(const QImage& img,int size)
{
    QImage padded=zeroPadding(img,size,size);
    QImage ret(padded);
    int width=ret.width();
    int height=ret.height();
    Matrix<int> tempR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(padded,'b');
    Matrix<int> returnR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> returnG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> returnB=Matrix<int>::fromQImage(padded,'b');
    for(int i=size/2;i<width-size/2;i++)
    {
        for(int j=size/2;j<height-size/2;j++)
        {
            Matrix<int> patchR=tempR.subMatrix(j-size/2,j+size/2,i-size/2,i+size/2);
            Matrix<int> patchG=tempG.subMatrix(j-size/2,j+size/2,i-size/2,i+size/2);
            Matrix<int> patchB=tempB.subMatrix(j-size/2,j+size/2,i-size/2,i+size/2);
//            std::vector<int> patchR=getPatch(ret,i,j,size,size,'r');
//            std::vector<int> patchG=getPatch(ret,i,j,size,size,'g');
//            std::vector<int> patchB=getPatch(ret,i,j,size,size,'b');
            double sumR=0,sumG=0,sumB=0;
//            std::sort(patchR.begin(),patchR.end());
//            std::sort(patchG.begin(),patchG.end());
//            std::sort(patchB.begin(),patchB.end());
//            qDebug()<<size*size-1;
            sumR=patchR.medium();
            sumG=patchG.medium();
            sumB=patchB.medium();
            int r=(int)sumR;
            int g=(int)sumG;
            int b=(int)sumB;
            if(r>255)
                r=255;
            if(g>255)
                g=255;
            if(b>255)
                b=255;
            if(r<0)
                r=0;
            if(g<0)
                g=0;
            if(b<0)
                b=0;
//            ret.setPixel(i,j,qRgb(r,g,b));
            returnR(j,i)=sumR;
            returnG(j,i)=sumG;
            returnB(j,i)=sumB;
        }
    }
    ret=Matrix<int>::toQImage(returnR,returnG,returnB);
    QImage croped=crop(ret,size,size);
    return croped;
}

QImage imageprocessor::dilationFilter(const QImage& img,int w,int h)
{
    QImage img1=rgb2gray(img);
    QImage padded=zeroPadding(img1,w,h);
//    QImage ret(padded);//操作的部分
    QImage dialted(padded);//最后赋值
    int width=padded.width();
    int height=padded.height();
    Matrix<int> tempR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(padded,'b');
    Matrix<int> returnR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> returnG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> returnB=Matrix<int>::fromQImage(padded,'b');
    for(int i=w/2;i<width-w/2;i++)
    {
        for(int j=h/2;j<height-h/2;j++)
        {
//            std::vector<int> patchR=getPatch(ret,i,j,w,h,'r');
//            std::vector<int> patchG=getPatch(ret,i,j,w,h,'g');
//            std::vector<int> patchB=getPatch(ret,i,j,w,h,'b');
            Matrix<int> patchR=tempR.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            Matrix<int> patchG=tempG.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            Matrix<int> patchB=tempB.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            int sumR=0,sumG=0,sumB=0;
//            sumR=*std::max_element(patchR.begin(),patchR.end());
//            sumG=*std::max_element(patchG.begin(),patchG.end());
//            sumB=*std::max_element(patchB.begin(),patchB.end());
            sumR=patchR.maxCal();
            sumG=patchG.maxCal();
            sumB=patchB.maxCal();
            int r=(int)sumR;
            int g=(int)sumG;
            int b=(int)sumB;
            if(r>255)
                r=255;
            if(g>255)
                g=255;
            if(b>255)
                b=255;
            if(r<0)
                r=0;
            if(g<0)
                g=0;
            if(b<0)
                b=0;
            returnR(j,i)=sumR;
            returnG(j,i)=sumG;
            returnB(j,i)=sumB;
        }
    }
    dialted=Matrix<int>::toQImage(returnR,returnG,returnB);
    QImage croped=crop(dialted,w,h);
    return croped;
}

QImage imageprocessor::erosionFilter(const QImage& img,int w,int h)
{
    QImage img1=rgb2gray(img);
    QImage padded=zeroPadding(img1,w,h);
//    QImage ret(padded);//操作的部分
    QImage dialted(padded);//最后赋值
    int width=padded.width();
    int height=padded.height();
    Matrix<int> tempR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> tempG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> tempB=Matrix<int>::fromQImage(padded,'b');
    Matrix<int> returnR=Matrix<int>::fromQImage(padded,'r');
    Matrix<int> returnG=Matrix<int>::fromQImage(padded,'g');
    Matrix<int> returnB=Matrix<int>::fromQImage(padded,'b');
    for(int i=w/2;i<width-w/2;i++)
    {
        for(int j=h/2;j<height-h/2;j++)
        {
//            std::vector<int> patchR=getPatch(ret,i,j,w,h,'r');
//            std::vector<int> patchG=getPatch(ret,i,j,w,h,'g');
//            std::vector<int> patchB=getPatch(ret,i,j,w,h,'b');
            Matrix<int> patchR=tempR.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            Matrix<int> patchG=tempG.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            Matrix<int> patchB=tempB.subMatrix(j-h/2,j+h/2,i-w/2,i+w/2);
            int sumR=0,sumG=0,sumB=0;
//            sumR=*std::max_element(patchR.begin(),patchR.end());
//            sumG=*std::max_element(patchG.begin(),patchG.end());
//            sumB=*std::max_element(patchB.begin(),patchB.end());
            sumR=patchR.minCal();
            sumG=patchG.minCal();
            sumB=patchB.minCal();
            int r=(int)sumR;
            int g=(int)sumG;
            int b=(int)sumB;
            if(r>255)
                r=255;
            if(g>255)
                g=255;
            if(b>255)
                b=255;
            if(r<0)
                r=0;
            if(g<0)
                g=0;
            if(b<0)
                b=0;
            returnR(j,i)=sumR;
            returnG(j,i)=sumG;
            returnB(j,i)=sumB;
        }
    }
    dialted=Matrix<int>::toQImage(returnR,returnG,returnB);
    QImage croped=crop(dialted,w,h);
    return croped;
}

QImage imageprocessor::affineTransform(const QImage &img,QMatrix mat)
{
    QMatrix inversedMat=mat.inverted();
    int x1=0,y1=0;
    int x2=img.width();
    int y2=0;
    int x3=0;
    int y3=img.height();
    int x4=img.width();
    int y4=img.height();
    int tx1,tx2,tx3,tx4,ty1,ty2,ty3,ty4;

    mat.map(x1,y1,&tx1,&ty1);
    mat.map(x2,y2,&tx2,&ty2);
    mat.map(x3,y3,&tx3,&ty3);
    mat.map(x4,y4,&tx4,&ty4);

    int maxX=tx1;
    if(tx2>maxX) maxX=tx2;
    if(tx3>maxX) maxX=tx3;
    if(tx4>maxX) maxX=tx4;

    int maxY=ty1;
    if(ty2>maxY) maxY=ty2;
    if(ty3>maxY) maxY=ty3;
    if(ty4>maxY) maxY=ty4;

    int minX=tx1;
    if(tx2<minX) minX=tx2;
    if(tx3<minX) minX=tx3;
    if(tx4<minX) minX=tx4;

    int minY=ty1;
    if(ty2<minY) minY=ty2;
    if(ty3<minY) minY=ty3;
    if(ty4<minY) minY=ty4;

    int width=maxX-minX;
    int height=maxY-minY;

    int deltaX=tx1-minX;
    int deltaY=ty1-minY;

    //需要平移回来
    QImage ret(width,height,QImage::Format_RGB32);
    for(int i=-deltaX;i<ret.width()-deltaX;i++)
    {
        for(int j=-deltaY;j<ret.height()-deltaY;j++)
        {
            double x,y;
            inversedMat.map(i,j,&x,&y);
            if(x>=0&&x<=img.width()-1&&y>=0&&y<=img.height())
            {
                ret.setPixel(i+deltaX,j+deltaY,imageprocessor::bilinearInput(x,y,img));
            }
            else ret.setPixel(i+deltaX,j+deltaY,qRgb(0,0,0));
        }
    }
    return ret;
}

QRgb imageprocessor::bilinearInput(double x,double y,const QImage &img)
{
    int r=0,g=0,b=0;
    QRgb rgb00=img.pixel((int)x,(int)y);
    QRgb rgb01=img.pixel((int)x+1,(int)y);
    QRgb rgb10=img.pixel((int)x,(int)y+1);
    QRgb rgb11=img.pixel((int)x+1,(int)y+1);
    Matrix<double> X(1,2,0);
    double x1=x-(int)x;
    double y1=y-(int)y;
    X(0,0)=1-x1;
    X(0,1)=x1;

    //注意图像坐标系和世界坐标系的差别
    Matrix<double> R(2,2,0);
    R(0,0)=(double)qRed(rgb10);
    R(0,1)=(double)qRed(rgb00);
    R(1,0)=(double)qRed(rgb11);
    R(1,1)=(double)qRed(rgb01);

    Matrix<double> Y(2,1,0);
    Y(0,0)=1-y1;
    Y(1,0)=y1;

    Matrix<double> G(2,2,0);
    G(0,0)=(double)qGreen(rgb10);
    G(0,1)=(double)qGreen(rgb00);
    G(1,0)=(double)qGreen(rgb11);
    G(1,1)=(double)qGreen(rgb01);

    Matrix<double> B(2,2,0);
    B(0,0)=(double)qBlue(rgb10);
    B(0,1)=(double)qBlue(rgb00);
    B(1,0)=(double)qBlue(rgb11);
    B(1,1)=(double)qBlue(rgb01);

    Matrix<double> rMat1=X*R;
    Matrix<double> rMat2=rMat1*Y;
    r=(int)rMat2(0,0);

    Matrix<double> gMat1=X*G;
    Matrix<double> gMat2=gMat1*Y;
    g=(int)gMat2(0,0);

    Matrix<double>bMat1=X*B;
    Matrix<double>bMat2=bMat1*Y;
    b=(int)bMat2(0,0);


    return QRgb(qRgb(r,g,b));
}

QImage imageprocessor::gaussBlurLP(const QImage &img,const int size,const double sigma)
{
    Matrix<int> im = Matrix<int>::fromQImage(img, 'G');
    Matrix<std::complex<double> > f = fft2d(im, im.getNRow(), im.getNCol());
    Matrix<std::complex<double> >::fftshift(f);

    // gauss filter
    Matrix<double> filter(f.getNRow(), f.getNCol(), 0);
    int width = filter.getNCol();
    int height = filter.getNRow();
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            double dis = (width/2-j)*(width/2-j) + (height/2-i)*(height/2-i);
            if(dis <= size *size){
                    filter(i, j) = exp((-1 * dis)/(2 * sigma * sigma));
            }else{
                filter(i,j)=0;
                }
            }
        }
    // multiplication
    Matrix<std::complex<double> > multiRes = Matrix<double>::multiplication(f, filter);
    // ifft2
    Matrix<std::complex<double> >::fftshift(multiRes);
    Matrix<std::complex<double> > ifftRes = ifft2d(multiRes, multiRes.getNRow(), multiRes.getNCol());
    Matrix<double> absRes = Matrix<int>::abs2stats(ifftRes);
    Matrix<int> res = Matrix<int>::normalization(absRes);
    // cut
    return Matrix<int>::toQImage(res.subMatrix(0, img.height(), 0, img.width()));
}

QImage imageprocessor::gaussBlurHP(const QImage &img,const int size,const double sigma)
{
    Matrix<int> im = Matrix<int>::fromQImage(img, 'G');
    Matrix<std::complex<double> > f = fft2d(im, im.getNRow(), im.getNCol());
    Matrix<std::complex<double> >::fftshift(f);

    // gauss filter
    Matrix<double> filter(f.getNRow(), f.getNCol(), 0);
    int width = filter.getNCol();
    int height = filter.getNRow();
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            double dis = (width/2-j)*(width/2-j) + (height/2-i)*(height/2-i);
            if(dis <= size *size){
                    filter(i, j) = 1-exp((-1 * dis)/(2 * sigma * sigma));
            }else{
                filter(i,j)=1;
                }
            }
        }

    // multiplication
    Matrix<std::complex<double> > multiRes = Matrix<double>::multiplication(f, filter);
    // ifft2
    Matrix<std::complex<double> >::fftshift(multiRes);
    Matrix<std::complex<double> > ifftRes = ifft2d(multiRes, multiRes.getNRow(), multiRes.getNCol());
    Matrix<double> absRes = Matrix<int>::abs2stats(ifftRes);
    Matrix<int> res = Matrix<int>::normalization(absRes);
    // cut
    return Matrix<int>::toQImage(res.subMatrix(0, img.height(), 0, img.width()));
}
