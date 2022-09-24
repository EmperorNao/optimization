#include "numerical.h"


Matrix<double> matrix_scalar_multiplication(Matrix<double>& A, double val) {

    if (A.size().size() != 2) {
        throw std::domain_error("Matrix in scalar multiplication must be 2 dimensional, "
                                "provided " + std::to_string(A.size().size())
                                );
    }

    Matrix<double> output(std::vector<uint64_t>{A.size()[0], A.size()[1]});
    for (uint64_t x = 0; x < output.size()[0]; ++x) {
        for (uint64_t y = 0; y < output.size()[1]; ++y) {
            output[{x, y}] = A[{x, y}] * val;
        }
    }

}


Matrix<double> matrix_addition(Matrix<double>& A, Matrix<double>& B) {

    if (A.size()[0] != B.size()[0] or A.size()[1] != B.size()[1]) {
        throw std::domain_error("Wrong shapes in matrix addition: " +
                                std::to_string(A.size()[0]) + "x" + std::to_string(A.size()[1]) + " * " +
                                std::to_string(B.size()[0]) + "x" + std::to_string(B.size()[1])
        );
    }

    Matrix<double> output(std::vector<uint64_t>{A.size()[0], A.size()[1]});
    for (uint64_t x = 0; x < output.size()[0]; ++x) {
        for (uint64_t y = 0; y < output.size()[1]; ++y) {
            output[{x, y}] = A[{x, y}] + B[{x, y}];
        }
    }

}


Matrix<double> matrix_product(Matrix<double>& A, Matrix<double>& B) {

    if (A.size()[1] != B.size()[0]) {
        throw std::domain_error("Wrong shapes in matrix product: " +
                                std::to_string(A.size()[0]) + "x" + std::to_string(A.size()[1]) + " * " +
                                std::to_string(B.size()[0]) + "x" + std::to_string(B.size()[1])
        );
    }

    Matrix<double> output(std::vector<uint64_t>{A.size()[0], B.size()[1]});
    for (uint64_t x = 0; x < output.size()[0]; ++x) {
        for (uint64_t y = 0; y < output.size()[1]; ++y) {
            output[{x, y}] = 0;
            for (uint64_t z = 0; z < A.size()[1]; ++z) {
                output[{x, y}] += A[{x, z}] * B[{y, z}];
            }
        }
    }

}


Matrix<double> linear_operator(Matrix<double>& data, Matrix<double>& A, Matrix<double>& B) {


    if (data.size().size() != 2 or A.size().size() != 2 or B.size().size() != 2) {
        throw std::domain_error("All matrixes in linear operator must be 2 dimensional, "
                                    "provided "
                                    "data.dims = " + std::to_string(data.size().size()) + " | "
                                    "A.dims = " + std::to_string(A.size().size()) + " | "
                                    "B.dims = " + std::to_string(B.size().size()) + " | "
                                    );
    }

    if (data.size()[1] != A.size()[0] or B.size()[1] != A.size()[1] or B.size()[0] != 1) {
        throw std::domain_error("Wrong shapes in linear operator: " +
            std::to_string(data.size()[0]) + "x" + std::to_string(data.size()[1]) + " * " +
            std::to_string(A.size()[0]) + "x" + std::to_string(A.size()[1]) + " + " +
            std::to_string(B.size()[0]) + "x" + std::to_string(B.size()[1])
        );
    }

    auto size = data.size();

    Matrix<double> output(std::vector<uint64_t>{data.size()[0], A.size()[1]});

    for (uint64_t x = 0; x < output.size()[0]; ++x) {
        for (uint64_t y = 0; y < output.size()[1]; ++y) {

            output[{x, y}] = 0;
            for (uint64_t z = 0; z < data.size()[1]; ++z) {
                output[{x, y}] += data[{x, z}] * A[{y, z}];
            }

            // add bias
            output[{x, y}] += B[{0, y}];

        }
    }

    return output;

}


void transpose(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("Matrixes in transposing must be 2 dimensional, "
                                "provided " + std::to_string(data.size().size())
        );
    }

    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = x + 1; y < data.size()[1]; ++y) {
            std::swap(data[{x, y}], data[{y, x}]);
        }
    }

}

