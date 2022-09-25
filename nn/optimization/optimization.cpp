#include "optimization.h"


void optimize(
        std::vector<Layer>& layers,
        std::vector<Layer>& grad,
        OPTIMIZATION_ALGO algorithm,
        std::map<std::string, std::string>& algorithm_parameters,
        bool verbose
) {

    switch (algorithm) {
        case OPTIMIZATION_ALGO::SGD:
            return sgd(layers, grad, algorithm_parameters, verbose);
        default:
            return sgd(layers, grad, algorithm_parameters, verbose);
    }

}
