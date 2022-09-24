#include "functional.h"



Matrix<double> ReLU::operator()(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("ReLU function expected data to be matrix with 2 dimensions, "
                                "provided " + std::to_string(data.size().size()));
    }

    Matrix<double> output(data.size());

    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] = std::max(0.0, data[{x, y}]);
        }
    }

    return output;

}


Matrix<double> ReLU::derivative(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("ReLU derivative expected data to be matrix with 2 dimensions, "
                                "provided " + std::to_string(data.size().size()));
    }

    Matrix<double> output(data.size());

    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] = data[{x, y}] > 0.0 ? 1.0 : 0.0;
        }
    }

    return output;

}


Matrix<double> Sigmoid::operator()(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("Sigmoid function expected data to be matrix with 1 dimension, "
                                "provided " + std::to_string(data.size().size()));
    }

    Matrix<double> output(data.size());

    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            output[{x, y}] = this->sigmoid(data[{x, y}]);
        }
    }

    return output;

}


Matrix<double> Sigmoid::derivative(Matrix<double>& data) {

    if (data.size().size() != 2) {
        throw std::domain_error("Sigmoid derivative expected data to be matrix with 1 dimension, "
                                "provided " + std::to_string(data.size().size()));
    }

    Matrix<double> output(data.size());

    for (uint64_t x = 0; x < data.size()[0]; ++x) {
        for (uint64_t y = 0; y < data.size()[1]; ++y) {
            double sigmoid = this->sigmoid(data[{x, y}]);
            output[{x, y}] = sigmoid * (1 - sigmoid);
        }
    }

    return output;

}


