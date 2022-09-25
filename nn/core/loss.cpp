#include "loss.h"


double BCELoss::value(Matrix<double>& predicted, Matrix<double>& labels) {

    if (predicted.size().size() != 2 or labels.size().size() != 2) {
        throw std::invalid_argument("IN BCELoss predicted and labels must have same size");
    }

    if (predicted.size()[1] != 1 or labels.size()[1] != 1 or predicted.size()[0] != labels.size()[0]) {
        throw std::invalid_argument("Wrong shapes in BCELoss value");
    }

    double value = 0;

    for (uint64_t idx = 0; idx < predicted.size()[0]; ++idx) {
        value += labels[{idx, 0}] * clamp_log(predicted[{idx, 0}]) + (1 - labels[{idx, 0}]) * clamp_log(1 - predicted[{idx, 0}]);
    }

    return -value;

}

Matrix<double> BCELoss::derivative(Matrix<double>& predicted, Matrix<double>& labels) {

    if (predicted.size().size() != 2 or labels.size().size() != 2) {
        throw std::invalid_argument("IN BCELoss predicted and labels must have same size");
    }

    if (predicted.size()[1] != 1 or labels.size()[1] != 1 or predicted.size()[0] != labels.size()[0]) {
        throw std::invalid_argument("Wrong shapes in BCELoss value");
    }

    Matrix<double> grad(std::vector<uint64_t>{predicted.size()[0], 1});

    for (uint64_t idx = 0; idx < predicted.size()[0]; ++idx) {
        grad[{idx, 0}] = (predicted[{idx, 0}] - labels[{idx, 0}]) / (1e-8 + (predicted[{idx, 0}] * (1 - predicted[{idx, 0}])));
    }

    return grad;

}
