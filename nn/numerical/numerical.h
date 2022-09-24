#ifndef NN_NUMERICAL_H
#define NN_NUMERICAL_H

#include "matrix.h"


//template <typename T>
//Matrix<T> max(Matrix<T> data) {
//
//    auto size = data.size();
//
//    if (size.size() > 2) {
//        throw std::invalid_argument("Max only for matrixes with less then 2 dimensions, "
//                                    "provided " + std::to_string(size.size()));
//    }
//
//    if (axis > data.size.size()) {
//        throw std::invalid_argument("Axis out of dimensions. Expected less then " +
//            std::to_string(size.size()) + ", provided" +
//            std::to_string(axis)
//            );
//    }
//
//    Matrix<T> output(data.size()[axis]);
//    std::vector<uint64_t> idxs(2, 0);
//    for (uint64_t idx = 0; idx < data.size()[axis]; ++idx) {
//
//        idxs[axis] = idx;
//        output[{idx}] = data[idxs];
//        for (uint64_t idx_to_max = 1; idx < data.size()[1 - axis]; ++idx_to_max) {
//            idxs[1 - axis] = idx_to_max;
//            output[{idx}] = output[{idx}] > data[idxs] ?  output[{idx}] : data[idxs];
//        }
//
//    }
//
//    return output;
//
//}


Matrix<double> matrix_scalar_multiplication(Matrix<double>& A, double val);
// return A * val


Matrix<double> matrix_addition(Matrix<double>& A, Matrix<double>& B);
// return A + B


Matrix<double> matrix_product(Matrix<double>& A, Matrix<double>& B);
// return A * B


Matrix<double> linear_operator(Matrix<double>& data, Matrix<double>& A, Matrix<double>& B);
// return data * A + B


void transpose(Matrix<double>& data);
// data = data.T


#endif //NN_NUMERICAL_H
