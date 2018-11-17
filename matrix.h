#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>
#include <vector>
#include <stdexcept>
#include <QImage>
#include <complex>
#include <QMatrix>

using std::range_error;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

template<typename T>
class Matrix
{
private:
    // point to the data
    T** _data;
    // number of row and column
    size_t _nRow, _nCol;
    // flag to indicate row-first or column-first
    int _t;
    bool checkRange(size_t r, size_t c) const;
    void swap(Matrix<T>& mat);
    size_t _startR,_startC;//表示矩阵的起点

public:
    // construct a r-row and c-column matrix with all init element
    Matrix(size_t r, size_t c, T init);
    // copy constructor
    Matrix(const Matrix<T>& mat);
    Matrix& operator=(Matrix<T> mat);
    ~Matrix();
    // get element at r-row and c-column
    // return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;
    Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;
    // output the matrix
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);
    size_t getNRow() const;
    size_t getNCol() const;
    size_t getR() const;
    size_t getC() const;
    void setStartR(const size_t sr);
    void setStartC(const size_t sc);
    std::vector<T> getRow(size_t r) const;
    std::vector<T> getCol(size_t c) const;
    bool setRow(size_t r, const std::vector<T>& d);
    bool setCol(size_t c, const std::vector<T>& d);
    // transpose it in-place
    Matrix<T>& transpose();

    /********矩阵加法**********/
    Matrix<T> operator+(const Matrix<T>& mat);//矩阵加法
    template<typename U>
    friend Matrix<U> operator+(U,const Matrix<U>& mat);//常数加矩阵
    template<typename U>
    friend Matrix<U> operator+(const Matrix<U>& mat，U);//矩阵加常数

    /********矩阵减法**********/
    Matrix<T> operator-(const Matrix<T>& mat);//矩阵减法
    template<typename U>
    friend Matrix<U> operator-(U,const Matrix<U>& mat);//常数减法
    template<typename U>
    friend Matrix<U> operator-(const Matrix<U>& mat,U);//常数减法

    /********矩阵乘法**********/
    Matrix<T> operator*(const Matrix<T>& mat);//矩阵乘法
    template<typename U>
    friend Matrix<U> operator*(U,const Matrix<U>& mat);//常数乘法
    template<typename U>
    friend Matrix<U> operator*(const Matrix<U>& mat,U);//常数乘法
    Matrix<T> operator/(T);

     /********矩阵和图像的转换**********/
    static Matrix<T> fromQImage(const QImage&, char);
    static QImage toQImage(const Matrix<int>&);
    static QImage toQImage(const Matrix<int>&, const Matrix<int>&, const Matrix<int>&);
    static Matrix<double> fromKernel(std::vector<double>,int,int);

    /********取模**********/
    static Matrix<double> abs2stats(const Matrix<std::complex<double>> &);

    /********对数变换**********/
    static Matrix<T> logTransformation(const Matrix<T> &, T );

    /********fftshift**********/
    static void fftshift(Matrix<T> &);

    /********归一化**********/
    static Matrix<int> normalization(const Matrix<double> &);

    /********求可逆矩阵**********/
//    static Matrix<T> matrixInverse(const Matrix<T>& ,bool *);

    //矩阵点乘
    static Matrix<T> multiplication(const Matrix<T> &, const Matrix<T> &);
    static Matrix<double> multiplication(const Matrix<int> &, const Matrix<double> &);
    static Matrix<std::complex<double> > multiplication(const Matrix<std::complex<double> > &, const Matrix<double> &);
    static Matrix<std::complex<double> > multiplication(const Matrix<double> &, const Matrix<std::complex<double> > &);

    static void kernelCal(const Matrix<double> &);
    T medium();
    T maxCal();
    T minCal();
};

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r), _nCol(c), _t(0)
{
    _t=1;//_t==1的时候代表没有转置
    // new _data
    // new _data[i] within loop
    _startC=0;
    _startR=0;
    _data=new T*[r];
    for(int i = 0; i < r; i++)
    {
        _data[i] = new T[c];
    }

    for(int i = 0; i < r ; i++){
        for (int j = 0; j < c; j++) {
            _data[i][j] = init;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t),_startR(mat._startR),_startC(mat._startC)
{
    // new _data
    // new and assign _data[i] within loop
    _t=1;
    _data=new T* [_nRow];
    for(int i=0;i<_nRow;i++)
    {
        _data[i]=new T[_nCol];
    }
    for(int i=0;i<_nRow;i++)
    {
        for(int j=0;j<_nCol;j++)
        {
            _data[i][j]=mat._data[i][j];
        }
    }
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& mat) {
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_startC, mat._startC);
    std::swap(_startR, mat._startR);
    std::swap(_t, mat._t);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}

template<typename T>
size_t Matrix<T>::getNRow() const
{
    // return the number of row
    if(_t==1)
        return _nRow;
    return _nCol;
}

template<typename T>
size_t Matrix<T>::getNCol() const
{
    // return the number of column
    if(_t==0)
        return _nRow;
    return _nCol;}

template<typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    // check if r-row and c-column is out of range
    //    cout<<_nRow<<" "<<_nCol<<endl;
    if(_t==1)
    {
        if(r>_nRow||c>_nCol)
            return false;
        //    cout<<"*****"<<_t<<"****"<<r<<" "<<c<<endl;
    }
    else if(_t==0)
    {
        if(r>=_nCol||c>=_nRow)
            return false;
    }
    return true;
}

template<typename T>
size_t Matrix<T>::getR() const
{
    if (_t == 1)
        return _startR;
    else
        return _startC;
}

template<typename T>
size_t Matrix<T>::getC() const
{
    if (_t == 1)
        return _startC;
    else
        return _startR;
}

template<typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    // get element in (r,c)
    // remember check first
    // different of _t has different order
    if(!checkRange(r, c))//越界
    {
        throw range_error("operator(): Error");
    }
    if(_t==1)
        return _data[(r+getR())%getNRow()][(c+getC())%getNCol()];
    else return _data[(c+getC())%getNCol()][(r+getR())%getNRow()];
}

template<typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    if (checkRange(rs, cs) && checkRange(re, ce) && rs <= re&&cs <= ce)
    {
        Matrix<T> submatrix(re - rs + 1, ce - cs + 1, 0);
        for (size_t i = 0; i < submatrix._nRow; i++)
        {
            for (size_t j = 0; j < submatrix._nCol; j++)
                //submatrix._data[i][j] = _data[i + rs][j + cs];
                submatrix(i,j) = operator()(i + rs,j + cs);
        }
        return submatrix;
    }
    else throw 0;
}
//Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
//{
//    // note the validity of value:rs, re, cs, ce
//    // try to use "except" to broadcast the exception
//    if(_t==1)
//    {
//        if(!(rs<=re&&re<_nRow&&cs<=ce&&ce<_nCol))
//        {
//            throw range_error("subMatrix:Error");
//        }
//        else
//        {
//            //            cout<<"re-rs"<<re-rs<<endl;
//            Matrix newMat=Matrix(re-rs,ce-cs,0);
//            //            cout<<newMat._nCol<<" "<<re-rs<<endl;
//            for(int i=0;i<newMat._nRow;i++)
//            {
//                for(int j=0;j<newMat._nCol;j++)
//                    newMat._data[i][j]=_data[i+rs][j+cs];
//            }
//            return newMat;
//        }
//    }
//    else
//    {
//        if(!(rs<=re&&re<_nCol&&cs<=ce&&ce<_nRow))
//        {
//            throw range_error("subMatrix:Error");
//        }
//        else
//        {
//            Matrix newMat=Matrix(ce-cs,re-rs,0);
//            for(int i=0;i<newMat._nRow;i++)
//            {
//                for(int j=0;j<newMat._nCol;j++)
//                    newMat._data[i][j]=_data[i+cs][j+rs];
//            }
//            return newMat;
//        }
//    }
//    return Matrix(1,1,0);
//}

template<typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    // get all element in r-row and push it back in a vector
    // remember check range first
    if((_t==1&&r>_nRow)||(_t==0&&r>_nCol))
    {
        throw range_error("getRow(): Error");
    }
    vector<T> vec;
    if(_t==1)
    {
        for(int i=0;i<_nCol;i++)
            vec.push_back(_data[r][i]);
    }
    else
    {
        for(int i=0;i<_nRow;i++)
            vec.push_back(_data[i][r]);
    }
    return vec;
}

template<typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    // get all element in c-column and push it back in a vector
    // remember check range first
    if((_t==1&&c>_nCol)||(_t==0&&c>_nRow))
    {
        throw range_error("getCol(): Error");
    }
    vector<T> vec;
    if(_t==1)
    {
        for(int i=0;i<_nRow;i++)
            vec.push_back(_data[i][c]);
    }
    else
    {
        for(int i=0;i<_nRow;i++)
            vec.push_back(_data[c][i]);
    }
    return vec;
}

template<typename T>
bool Matrix<T>::setRow(size_t r, const std::vector<T>& d)
{
    // set the element of the r-row
    // remember check range first
    if(_t==1&&checkRange(r, d.size()-1))
    {
        if(d.size()==_nCol)
        {
            for(int i=0;i<d.size();i++)
                _data[r][i]=d[i];
            return true;
        }
        else throw range_error("setRow: Error");
    }
    if(_t==0&&checkRange(d.size()-1, r))
    {
        if(d.size()==_nRow)
        {
            for(int i=0;i<d.size()-1;i++)
                _data[i][r]=d[i];
            return true;
        }
        else throw range_error("setRow: Error");
    }
    return false;
}

template<typename T>
bool Matrix<T>::setCol(size_t c, const std::vector<T>& d)
{
    // set the element of the c-column
    // remember check range first
    if(_t==1&&checkRange(d.size()-1,c))
    {
        if(d.size()==_nRow)
        {
            for(int i=0;i<d.size();i++)
                _data[i][c]=d[i];
            return true;
        }
        else throw range_error("setCol: Error");
    }
    if(_t==0&&checkRange(c,d.size()-1))
    {
        if(d.size()==_nCol)
        {
            for(int i=0;i<d.size()-1;i++)
                _data[c][i]=d[i];
            return true;
        }
        else throw range_error("setRow: Error");
    }
    return false;
}

template<typename T>
Matrix<T>::~Matrix()
{
    for(int i=0;i<_nRow;i++)
    {
        delete _data[i];
    }
    delete _data;
    _t=0;
}

template<typename T>
Matrix<T>& Matrix<T>::transpose()
{
    // change _t
    //    // swap _nRow and _nCol
    if(_t==0)_t=1;
    else _t=0;
    //    size_t temp=_nCol;
    //    _nCol=_nRow;
    //    _nRow=temp;
    //    std::swap(_nCol,_nRow);
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
{
    for(size_t i=0; i<rhs.getNRow(); ++i) {
        for(size_t j=0; j<rhs.getNCol(); ++j) {
            out << rhs(i,j) << "\t";
        }
        out << endl;
    }
    return out;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat)//矩阵加法
{
    Matrix<T> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j=0;j<_nCol;j++)
            ret(i,j)=operator()(i,j)+mat(i,j);
    }
    return ret;
}

template<typename U>
Matrix<U> operator+(U lhs,const Matrix<U>& mat)//矩阵加法,常数加矩阵
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=lhs+mat(i,j);
    }
    return ret;
}

template<typename U>
Matrix<U> operator+(const Matrix<U>& mat,U rhs)//矩阵加法,常数加矩阵
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=rhs+mat(i,j);
    }
    return ret;
}


template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &mat)//矩阵减法
{
    Matrix<T> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j=0;j<_nCol;j++)
            ret(i,j)=operator()(i,j)-mat(i,j);
    }
    return ret;
}

template<typename U>
Matrix<U> operator-(U rhs,const Matrix<U> &mat)//常数减矩阵
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=rhs-mat(i,j);
    }
    return ret;
}

template<typename U>
Matrix<U> operator-(const Matrix<U> &mat,U lhs)//常数减矩阵
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=mat(i,j)-lhs;
    }
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &mat)//矩阵乘法
{
    //_nRow为原来矩阵的行，mat._nCol为输入矩阵的列
    Matrix<T> ret(getNRow(),mat.getNCol(),0);
    for(size_t i=0;i<getNRow();i++){
        for(size_t j=0;j<mat.getNCol();j++){
            //ret(m,s)=0;//变量使用前记得初始化,否则结果具有不确定性
            for(size_t k=0;k<getNCol();k++){
                ret(i,j)+=operator()(i,k)*mat(k,j);
            }
        }
    }
    return ret;
}

template<typename U>
Matrix<U> operator*(U lhs,const Matrix<U> &mat)//常数×矩阵
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=lhs*mat(i,j);
    }
    return ret;
}

template<typename U>
Matrix<U> operator*(const Matrix<U> &mat,U rhs)//矩阵乘常数
{
    Matrix<U> ret(mat._nRow,mat._nCol,mat(0,0));
    for(size_t i=0;i<mat._nRow;i++)
    {
        for(size_t j=0;j<mat._nCol;j++)
            ret(i,j)=rhs*mat(i,j);
    }
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(T d)//矩阵加法
{
    Matrix<T> ret(_nRow,_nCol,operator()(0,0));
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j=0;j<_nCol;j++)
            ret(i,j)=operator()(i,j)/d;
    }
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::fromQImage(const QImage& img,char temp)
{
    Matrix<T> ret(img.height(),img.width(),0);
    for(int i=0;i<img.height();i++)
    {
        for(int j=0;j<img.width();j++)
        {
            if(temp=='r')
                ret(i,j)=qRed(img.pixel(j,i));
            if(temp=='g')
                ret(i,j)=qGreen(img.pixel(j,i));
            if(temp=='b')
                ret(i,j)=qBlue(img.pixel(j,i));
            if(temp=='G')
                ret(i,j)=qGray(img.pixel(j,i));
        }
    }
    return ret;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& mat)
{
    int width=mat.getNCol();
    int height=mat.getNRow();
    QImage img=QImage(width,height,QImage::Format_RGB32);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int temp=mat(j,i);
            img.setPixel(i,j,qRgb(temp,temp,temp));
        }
    }
    return img;
}

template <typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& red,const Matrix<int>& green,const Matrix<int>& blue)
{
    int width=red.getNCol();
    int height=red.getNRow();
    QImage img=QImage(width,height,QImage::Format_RGB32);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int r=red(j,i);
            int g=green(j,i);
            int b=blue(j,i);
            img.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return img;
}

template <typename T>
Matrix<double> Matrix<T>::abs2stats(const Matrix<std::complex<double>> & mat)
{
    int width=mat.getNCol();
    int height=mat.getNRow();
    Matrix<double> res(height,width,0);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
            res(i,j)=abs(mat(i,j));
    }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::logTransformation(const Matrix<T> &mat, T c)
{
    int width=mat.getNCol();
    int height=mat.getNRow();
    Matrix<T> res(height,width,0);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
            res(i,j)=c*log(1+mat(i,j));
    }
    return res;
}

template <typename T>
void Matrix<T>::setStartR(const size_t sr)
{
    this->_startR=sr;
}

template <typename T>
void Matrix<T>::setStartC(const size_t sc)
{
    this->_startC=sc;
}

template <typename T>
void Matrix<T>::fftshift(Matrix<T> & mat)
{
    mat.setStartC(mat.getNCol()/2);
    mat.setStartR(mat.getNRow()/2);
}

template <typename T>
Matrix<int> Matrix<T>::normalization(const Matrix<double> &mat)
{
    int width=mat.getNCol();
    int height=mat.getNRow();
    Matrix<T> res(height,width,0);
    double max=mat(0,0);
    double min=mat(0,0);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(mat(i,j)>max)max=mat(i,j);
            if(mat(i,j)<min)min=mat(i,j);
        }
    }
    double deno=max-min;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
            res(i,j)=(int)((mat(i,j)-min)/deno*255);
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::multiplication(const Matrix<T> &mat1, const Matrix<T> &mat2)
{
    if((mat1.getNCol() == mat2.getNCol()) && (mat1.getNRow() == mat2.getNRow())){
        Matrix<T> matRes(mat1.getNRow(), mat1.getNCol(), 0);
        for(int i = 0; i < mat1.getNRow(); i++){
            for(int j = 0; j < mat1.getNCol(); j++){
                matRes(i, j) = mat1(i, j) * mat2(i, j);
            }
        }

        return matRes;
    }else{
        throw range_error("multiplication :    illegal size of matrix");
    }
}

template<typename T>
Matrix<std::complex<double> > Matrix<T>::multiplication(const Matrix<std::complex<double> > & matCom, const Matrix<double> & matDou)
{
    if(matCom.getNCol() != matDou.getNCol() || matCom.getNRow() != matDou.getNRow())
    {
        throw range_error("mulComplexByDouble :    illigal matrix size");
    }
    Matrix<std::complex<double>> retMat(matDou.getNRow(), matDou.getNCol(), 0);
    for(int i = 0; i < matDou.getNRow(); i++){
        for(int j = 0; j < matDou.getNCol(); j++){
            retMat(i, j) = matCom(i, j) * matDou(i, j);
        }
    }

    return retMat;
}

template<typename T>
Matrix<std::complex<double> > Matrix<T>::multiplication(const Matrix<double> & matDou, const Matrix<std::complex<double> > & matCom)
{

    return multiplication(matCom, matDou);
}

template<typename T>
Matrix<double> Matrix<T>::fromKernel(std::vector<double> kernel,int nCol,int nRow)
{
    Matrix<double> mat(nRow,nCol,0);
    int k=0;
    for(int i=0;i<nRow;i++)
    {
        for(int j=0;j<nCol;j++)
        {
            mat(i,j)=kernel[k];
            k++;
        }
//            mat(i,j)=kernel[i+j];
    }
    return mat;
}

template<typename T>
Matrix<double> Matrix<T>::multiplication(const Matrix<int> & matDou, const Matrix<double> & matCom)
{
    if(matCom.getNCol() != matDou.getNCol() || matCom.getNRow() != matDou.getNRow())
    {
        throw range_error("mulComplexByDouble :    illigal matrix size");
    }
    Matrix<double> retMat(matDou.getNRow(), matDou.getNCol(), 0);
    for(int i = 0; i < matDou.getNRow(); i++){
        for(int j = 0; j < matDou.getNCol(); j++){
            retMat(i, j) = matCom(i, j) * matDou(i, j);
        }
    }
    return retMat;
}

template<typename T>
void Matrix<T>::kernelCal(const Matrix<double>& kernel)
{
    double sum=0;
    for(int i=0;i<kernel.getNCol();i++)
    {
        for(int j=0;j<kernel.getNRow();j++)
            sum+=kernel(i,j);
    }
    if(0!=sum)
    {
        for(int i=0;i<kernel.getNCol();i++)
        {
            for(int j=0;j<kernel.getNRow();j++)
                kernel(i,j)/=sum;
        }
    }
}

template<typename T>
T Matrix<T>::medium()
{
    std::vector<T> temp;
    for(int i=0;i<getNRow();i++)
    {
        for(int j=0;j<getNCol();j++)
            temp.push_back(operator ()(i,j));
    }
    int flag;
    int size=temp.size();
        while(size>1)
        {
            for(int i=0;i<size-1;i++)
            {
                if(temp[i]>temp[i+1])
                {
                    flag=temp[i];
                    temp[i]=temp[i+1];
                    temp[i+1]=flag;
                }
            }
            size--;
        }
        return temp[temp.size()/2+1];
}

template<typename T>
T Matrix<T>::maxCal()
{
    T temp = -1000000;
    for (int i = 0; i<_nRow; i++)
        for (int j = 0; j<_nCol; j++)
            if (_data[i][j]>temp)
                temp = _data[i][j];
    return temp;
}

template<typename T>
T Matrix<T>::minCal()
{
    T temp = 1000000;
    for (int i = 0; i<_nRow; i++)
        for (int j = 0; j<_nCol; j++)
            if (_data[i][j]<temp)
                temp = _data[i][j];
    return temp;
}
//Matrix<T> Matrix<T>::matrixInverse(const Matrix<T> &mat,bool *tempInverse)
//{
//    if(mat.getNCol()!=3||mat.getNRow()!=3)
//    {
//        *tempInverse=false;//不可逆
//        return Matrix(mat);
//    }
//    //计算矩阵行列式，行列式为0则不可逆
//    //a1(b2c3-b3c2) - a2(b1c3-b3c1) + a3(b1c2-b2c1)
//    T result1=mat(0,0)*mat(1,1)*mat(2,2)-mat(0,0)*mat(1,2)*mat(2,1);
//    T result2=mat(0,1)*mat(1,0)*mat(2,2)-mat(0,1)*mat(1,2)*mat(2,0);
//    T result3=mat(0,2)*mat(1,0)*mat(2,1)-mat(0,2)*mat(1,1)*mat(2,0);
//    T determinant=result1-result2+result3;
//    if(determinant==0)
//    {
//        *tempInverse=false;//不可逆
//        return Matrix(mat);
//    }
//    *tempInverse=true;
//    Matrix<T> inverseMat=QMatrix::inverted(*tempInverse);
//    return inverseMat;
//}
#endif // MATRIX_H
