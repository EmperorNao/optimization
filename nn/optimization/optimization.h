#ifndef NN_OPTIMIZATION_H
#define NN_OPTIMIZATION_H

#include <map>

#include "../core/layer.h"
#include "sgd.h"


enum class OPTIMIZATION_ALGO {
    SGD
};


void optimize(
        std::vector<Layer>& layers,
        std::vector<Matrix<double>>& inputs,
        Matrix<double>& grad,
        OPTIMIZATION_ALGO algorithm,
        std::map<std::string, std::string>& algorithm_parameters
        ) {

    switch (algorithm) {
        case OPTIMIZATION_ALGO::SGD:
            return sgd(layers, inputs, grad, algorithm_parameters);
        default:
            return sgd(layers, inputs, grad, algorithm_parameters);
    }

}


#endif //NN_OPTIMIZATION_H