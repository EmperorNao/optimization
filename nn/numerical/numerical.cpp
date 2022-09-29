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
    return output;

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
    return output;

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
                output[{x, y}] += A[{x, z}] * B[{z, y}];
            }
        }
    }
    return output;

}


Matrix<double> dot_product(Matrix<double>& A, Matrix<double>& B) {

    if (A.size()[0] != B.size()[0] or A.size()[1] != B.size()[1]) {
        throw std::domain_error("Wrong shapes in dot product: " +
                                std::to_string(A.size()[0]) + "x" + std::to_string(A.size()[1]) + " * " +
                                std::to_string(B.size()[0]) + "x" + std::to_string(B.size()[1])
        );
    }

    Matrix<double> output(std::vector<uint64_t>{A.size()[0], A.size()[1]});
    for (uint64_t x = 0; x < output.size()[0]; ++x) {
        for (uint64_t y = 0; y < output.size()[1]; ++y) {
            output[{x, y}] = A[{x, y}] * B[{x, y}];
        }
    }
    return output;

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
                output[{x, y}] += data[{x, z}] * A[{z, y}];
            }

            // add bias
            output[{x, y}] += B[{0, y}];

        }
    }

    return output;

}


Matrix<double> transpose(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("Matrixes in transposing must be 2 dimensional, "
                                "provided " + std::to_string(data.size().size())
        );
    }

    Matrix<double> output(std::vector<uint64_t>{data.size()[1], data.size()[0]});
    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{y, x}] = data[{x, y}];
        }
    }
    return output;

}


Matrix<double> map(Matrix<double>& data, std::function<double(double)> func) {

    if (data.size().size() != 2) {
        throw std::domain_error("Matrixes in map must be 2 dimensional, "
                                "provided " + std::to_string(data.size().size())
        );
    }

    Matrix<double> output(std::vector<uint64_t>{data.size()[0], data.size()[1]});
    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] = func(data[{x, y}]);
        }
    }
    return output;

}


Matrix<double> mean(Matrix<double>& data, uint64_t axis) {

    auto size = data.size();

    if (size.size() != 2) {
        throw std::invalid_argument("Mean only for matrixes with 2 dimensions, "
                                    "provided " + std::to_string(size.size()));
    }

    if (axis > data.size().size()) {
        throw std::invalid_argument("Axis out of dimensions. Expected less then " +
                                    std::to_string(size.size()) + ", provided" +
                                    std::to_string(axis)
        );
    }

    Matrix<double> output(axis? std::vector<uint64_t>{1, data.size()[1]} : std::vector<uint64_t>{data.size()[0], 1});

    if (axis) {

        for (uint64_t idx = 0; idx < data.size()[1]; ++idx) {

            output[{0, idx}] = 0;
            for (uint64_t idx_to_sum = 1; idx_to_sum < data.size()[0]; ++idx_to_sum) {
                output[{0, idx}] += data[{idx_to_sum, idx}];
            }
            output[{0, idx}] /= data.size()[0];

        }

    }
    else {

        for (uint64_t idx = 0; idx < data.size()[0]; ++idx) {

            output[{idx, 0}] = 0;
            for (uint64_t idx_to_sum = 1; idx_to_sum < data.size()[1]; ++idx_to_sum) {
                output[{idx, 0}] += data[{idx, idx_to_sum}];
            }
            output[{idx, 0}] /= data.size()[1];

        }

    }

    return output;

}


Matrix<double> argmax(Matrix<double>& data, uint64_t axis) {

    auto size = data.size();

    if (size.size() != 2) {
        throw std::invalid_argument("Mean only for matrixes with 2 dimensions, "
                                    "provided " + std::to_string(size.size()));
    }

    if (axis > data.size().size()) {
        throw std::invalid_argument("Axis out of dimensions. Expected less then " +
                                    std::to_string(size.size()) + ", provided" +
                                    std::to_string(axis)
        );
    }

    Matrix<double> output;

    if (axis) {

        output.resize({1, data.size()[1]});
        for (uint64_t idx = 0; idx < data.size()[1]; ++idx) {

            output[{0, idx}] = 0;
            double max = data[{0, idx}];
            for (uint64_t idx_to_sum = 1; idx_to_sum < data.size()[0]; ++idx_to_sum) {
                if (data[{idx_to_sum, idx}] > max) {
                    output[{0, idx}] = idx_to_sum;
                    max = data[{idx_to_sum, idx}];
                }
            }

        }

    }
    else {

        output.resize({data.size()[0], 1});
        for (uint64_t idx = 0; idx < data.size()[0]; ++idx) {

            output[{idx, 0}] = 0;
            double max = data[{idx, 0}];
            for (uint64_t idx_to_sum = 1; idx_to_sum < data.size()[1]; ++idx_to_sum) {
                if (data[{idx, idx_to_sum}] > max) {
                    output[{idx, 0}] = idx_to_sum;
                    max = data[{idx, idx_to_sum}];
                }
            }

        }

    }

    return output;

}


Matrix<double> softmax(Matrix<double>& data) {

    Matrix<double> output(data.size());

    for (uint64_t x = 0; x < data.size()[0]; ++x) {

        double max_val = data[{x, 0}];
        for (uint64_t y = 1; y < data.size()[1]; ++y) {
            max_val = std::max(max_val, data[{x, y}]);
        }

        double sum = 0;
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] = std::exp(data[{x, y}] - max_val);
            sum +=  output[{x, y}];
        }

        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] /= sum;
        }

    }

    return output;

}

