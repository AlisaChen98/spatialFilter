#ifndef FFT__H
#define FFT__H

#include <complex>
#include <vector>
#include  "matrix.h"


const double PI = 3.1415926;

size_t calcN(size_t length);
//std::complex<double> pow(std::complex<double> base, int exponent);
std::complex<double> wnk(size_t, int);
std::complex<double> wnkMinus(size_t, int);
// different function with different input
vector<std::complex<double> > fftCal(vector<std::complex<double> > data, size_t N, bool flag);
std::vector<std::complex<double> > fft(std::vector<int> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<double> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<std::complex<double> > data, size_t N=0) ;
std::vector<std::complex<double> > ifft(std::vector<int> data, size_t N=0);
std::vector<std::complex<double> > ifft(std::vector<double> data, size_t N=0);
std::vector<std::complex<double> > ifft(std::vector<std::complex<double> > data, size_t N=0);
Matrix<std::complex<double> > fftRow(const Matrix<std::complex<double> >& mat, size_t row, size_t col,bool flag);
Matrix<std::complex<double> > newMatrix(const Matrix<std::complex<double> >& mat, size_t row, size_t col);
Matrix<std::complex<double> > fft2d(const Matrix<int>& mat, size_t row, size_t col);
Matrix<std::complex<double> > fft2d(const Matrix<double>& mat, size_t row, size_t col);
Matrix<std::complex<double> > fft2d(const Matrix<std::complex<double> >& mat, size_t row, size_t col);
Matrix<std::complex<double> > ifft2d(const Matrix<int>& mat, size_t row, size_t col);
Matrix<std::complex<double> > ifft2d(const Matrix<double>& mat, size_t row, size_t col);
Matrix<std::complex<double> > ifft2d(const Matrix<std::complex<double> >& mat, size_t row, size_t col);


#endif
