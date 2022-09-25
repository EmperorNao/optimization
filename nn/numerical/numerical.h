#ifndef NN_NUMERICAL_H
#define NN_NUMERICAL_H

#include <functional>

#include "matrix.h"


Matrix<double> matrix_scalar_multiplication(Matrix<double>& A, double val);
// return A * val


Matrix<double> matrix_addition(Matrix<double>& A, Matrix<double>& B);
// return A + B


Matrix<double> matrix_product(Matrix<double>& A, Matrix<double>& B);
// return A * B


Matrix<double> dot_product(Matrix<double>& A, Matrix<double>& B);
// return A dot B


Matrix<double> linear_operator(Matrix<double>& data, Matrix<double>& A, Matrix<double>& B);
// return data * A + B


Matrix<double> map(Matrix<double>& data, std::function<double(double)> func);
// return map(data)


Matrix<double> transpose(Matrix<double>& data);
// return data.T


Matrix<double> mean(Matrix<double>& data, uint64_t axis = 0);


#endif //NN_NUMERICAL_H
