#include "fft.h"

#define PI 3.1415926535898

using std::complex;
using std::vector;


size_t calcN(size_t length) {
  // check if length is power of 2
  // if it is, just return length
  // if not, get the correct N and return
    if(0==(length&(length-1)))//按二进制按位与，两个二进位都为1则为1
        return length;
    vector<size_t> vec;
    while(length){
        vec.push_back(length);
        length=length>>1;//右移运算，左边空位补零
    }
    size_t res=vec[0];
    for(int i=1;i<vec.size();i++)
    {
        res|=vec[i];//按位或，有一个为1就为1
    }
    return res+1;
}

complex<double> wnk(size_t N, int k){
    // calc the W_{N}^{k}
    double p = 2 * PI * k /N;
    return complex<double>(cos(p), -1 * sin(p));
}

complex<double> wnkMinus(size_t N, int k){
    // calc the W_{N}^{-k}
    double p = -2 * M_PI * k /N;
    return complex<double>(cos(p), -1 * sin(p));
}

vector<complex<double> >
fftCal(vector<complex<double> > data, size_t N, bool flag) {
    
    //         true: fft
    //         false: ifft
    
    // change length to make it beign just the power of 2
    N = calcN(N);
    // append 0 if necessary
    while (N > data.size()){
        data.push_back(complex<double>(0.0, 0.0));
    }
    vector<complex<double> > res;
    if(N == 1 || N == 0){
        return data;
    }else if(N == 2){
        if(!flag){
            res.push_back(wnkMinus(2, 0)*data[0] + wnkMinus(2, 0)*data[1]);
            res.push_back(wnkMinus(2, 0)*data[0] + wnkMinus(2, 1)*data[1]);
        }else if(flag){
            res.push_back(wnk(2, 0)*data[0] + wnk(2, 0)*data[1]);
            res.push_back(wnk(2, 0)*data[0] + wnk(2, 1)*data[1]);
        }
        return res;
    }else{
        // split
        vector<complex<double> > evenItems;
        vector<complex<double> > oddItems;
        for(int i = 0; i < data.size(); i++){
            if(i % 2 == 0){
                evenItems.push_back(data[i]);
            }else{
                oddItems.push_back(data[i]);
            }
        }
        
        vector<complex<double> > evenRes = fftCal(evenItems, N/2, flag);
        vector<complex<double> > oddRes = fftCal(oddItems, N/2, flag);
        
        // construct
        for(int i = 0; i < N/2; i++){
            if(!flag){
                res.push_back(evenRes[i] +wnkMinus(N, i) * oddRes[i]);
            }else if(flag){
                res.push_back(evenRes[i] + wnk(N, i) * oddRes[i]);
            }
        }
        for(int i = 0; i < N/2; i++){
            if(!flag){
                res.push_back(evenRes[i] - wnkMinus(N, i) * oddRes[i]);
            }else if(flag){
                res.push_back(evenRes[i] - wnk(N, i) * oddRes[i]);
            }
        }
    }
    
    return res;
}

vector<complex<double> >
fft(vector<int> data, size_t N)
{
  // convert data to vector<complex<double> >
  // call:
  // vector<complex<double> >
  // fft(vector<complex<double> > data, size_t N)
    vector<complex<double>> complexData;
    for(int i=0;i<N;i++)
        complexData.push_back(complex<double>(data[i],0.0));
    return fft(complexData,N);
}

vector<complex<double> >
fft(vector<double> data, size_t N)
{

  // convert data to vector<complex<double> >
  // call:
  // vector<complex<double> >
  // fft(vector<complex<double> > data, size_t N)
    vector<complex<double>> complexData;
    for(int i=0;i<N;i++)
        complexData.push_back(complex<double>(data[i],0.0));
    return fft(complexData,N);
}

vector<complex<double> >
fft(vector<complex<double> > data, size_t N) {
    return fftCal(data, N, true);
}

std::vector<std::complex<double> > ifft(std::vector<int> data, size_t N)
{
    vector<complex<double>> complexData;
    for(int i=0;i<N;i++)
        complexData.push_back(complex<double>(data[i],0.0));
    return ifft(complexData,N);
}

std::vector<std::complex<double> > ifft(std::vector<double> data, size_t N)
{
    vector<complex<double>> complexData;
    for(int i=0;i<N;i++)
        complexData.push_back(complex<double>(data[i],0.0));
    return ifft(complexData,N);
}

vector<complex<double> >
ifft(vector<complex<double> > data, size_t N){
    std::vector<complex<double> > res =  fftCal(data, N, false);
    for(int i = 0; i < res.size(); i++){
        res.at(i) /= calcN(N);
    }
    return res;
}

Matrix<complex<double> >
newMatrix(const Matrix<complex<double> >& mat, size_t row, size_t col){

    size_t tempRow = calcN(row);
    size_t tempCol = calcN(col);
    Matrix<complex<double> > m(tempRow, tempCol, complex<double>(0, 0));

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            m(i, j) = mat(i, j);
        }
    }
    return m;
}

Matrix<complex<double> >
fftRow(const Matrix<std::complex<double> >& mat, size_t row, size_t col,bool flag){
    
    //         true: fft
    //         false: ifft
    Matrix<complex<double> > newMat(row, col, complex<double>(0,0));
    for(int i = 0; i < row; i++){
        vector<complex<double>> vecRow;
        if(flag)
        {
            vecRow = fft(mat.getRow(i), col);
            newMat.setRow(i, vecRow);
        }
        if(!flag)
        {
            vecRow = ifft(mat.getRow(i), col);
            newMat.setRow(i, vecRow);
        }
//        for(size_t i=0; i<vecRow.size(); ++i)
//            cout << vecRow.at(i) << "\t";
//        cout<<endl;
    }
    return newMat;
}


Matrix<complex<double>> fft2d(const Matrix<complex<double> >& data, size_t N, size_t M){
      Matrix<complex<double>> data2=newMatrix(data,N,M);
      size_t nN=calcN(N);
      size_t nM=calcN(M);
      Matrix<complex<double>> newdata=fftRow(data2,nN,nM,true);
      newdata.transpose();
      Matrix<complex<double>> newdata2=fftRow(newdata,nM,nN,true);
      newdata2.transpose();
      return newdata2;
}

Matrix<complex<double> >
fft2d(const Matrix<int>& mat, size_t row, size_t col){
    Matrix<std::complex<double> > comMat(row, col, complex<double>(0, 0));
    
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            comMat(i, j) = complex<double>(mat(i, j), 0);
        }
    }
    return fft2d(comMat, row, col);
}

Matrix<complex<double> >
fft2d(const Matrix<double>& mat, size_t row, size_t col){
    Matrix<std::complex<double> > comMat(row, col, complex<double>(0, 0));
    
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            comMat(i, j) = complex<double>(mat(i, j), 0);
        }
    }
    return fft2d(comMat, row, col);
}

Matrix<std::complex<double> > ifft2d(const Matrix<int>& mat, size_t row, size_t col)
{
    Matrix<std::complex<double> > comMat(row, col, complex<double>(0, 0));
    
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            comMat(i, j) = complex<double>(mat(i, j), 0);
        }
    }
    return ifft2d(comMat, row, col);
}

Matrix<std::complex<double>> ifft2d(const Matrix<double>& mat, size_t row, size_t col)
{
    Matrix<std::complex<double> > comMat(row, col, complex<double>(0, 0));
    
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            comMat(i, j) = complex<double>(mat(i, j), 0);
        }
    }
    return ifft2d(comMat, row, col);
}

Matrix<complex<double>> ifft2d(const Matrix<complex<double> >& data, size_t N, size_t M)
{
    Matrix<complex<double>> data2=newMatrix(data,N,M);
    size_t nN=calcN(N);
    size_t nM=calcN(M);
    Matrix<complex<double>> newdata=fftRow(data2,nN,nM,false);
    newdata.transpose();
    Matrix<complex<double>> newdata2=fftRow(newdata,nM,nN,false);
    newdata2.transpose();
    return newdata2;
}


