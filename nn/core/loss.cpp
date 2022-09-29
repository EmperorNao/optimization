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


double CELossWithLogits::value(Matrix<double>& predicted, Matrix<double>& labels) {

    if (predicted.size().size() != 2 or labels.size().size() != 2) {
        throw std::invalid_argument("IN CDELoss predicted and labels must have same size");
    }

    if (predicted.size()[0] != labels.size()[0]) {
        throw std::invalid_argument("Wrong shapes in BCELoss value");
    }

    double value = 0;

    predicted = softmax(predicted);
    for (uint64_t idx = 0; idx < predicted.size()[0]; ++idx) {
        for (uint64_t class_idx = 0; class_idx < predicted.size()[1]; ++class_idx) {
            value += (labels[{idx, class_idx}] == 1) * clamp_log(predicted[{idx, class_idx}]);
        }
    }

    return -value;

}

Matrix<double> CELossWithLogits::derivative(Matrix<double>& predicted, Matrix<double>& labels) {

    if (predicted.size().size() != 2 or labels.size().size() != 2) {
        throw std::invalid_argument("IN CELoss predicted and labels must have same size");
    }

    if (predicted.size()[0] != labels.size()[0]) {
        throw std::invalid_argument("Wrong shapes in CELoss value");
    }

    predicted = softmax(predicted);
    Matrix<double> grad(predicted.size());

    for (uint64_t idx = 0; idx < predicted.size()[0]; ++idx) {
        for (uint64_t class_idx = 0; class_idx < predicted.size()[1]; ++class_idx) {
            grad[{idx, class_idx}] = predicted[{idx, class_idx}] - labels[{idx, class_idx}];
        }
    }

    return grad;

}
