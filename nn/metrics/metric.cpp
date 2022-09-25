#include "metric.h"


double Accuracy::value(Matrix<double>& predicted, Matrix<double>& labels) {

    if (predicted.size().size() != 2 or labels.size().size() != 2) {
        throw std::invalid_argument("IN Accuracy predicted and labels must have same size");
    }

    if (predicted.size()[1] != 1 or labels.size()[1] != 1 or predicted.size()[0] != labels.size()[0]) {
        throw std::invalid_argument("Wrong shapes in Accuracy value");
    }

    double correct = 0;
    for (uint64_t idx = 0; idx < predicted.size()[0]; ++idx) {

        correct += predicted[{idx, 0}] == labels[{idx, 0}];

    }

    return correct / predicted.size() [0];

}