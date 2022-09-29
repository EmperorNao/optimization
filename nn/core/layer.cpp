#include "layer.h"


Matrix<double> BatchNorm::function(Matrix<double>& data, double mean, double var) {
    return data;
}

Layer BatchNorm::derivative(Matrix<double>& data, double mean, double gamma) {
    return Layer(ACTIVATION::Identity);
}
