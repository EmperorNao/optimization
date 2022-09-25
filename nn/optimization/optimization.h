#ifndef NN_OPTIMIZATION_H
#define NN_OPTIMIZATION_H

#include <map>


#include "../core/layer.h"
#include "../core/layer.h"
#include "../numerical/numerical.h"


enum class OPTIMIZATION_ALGO {
    SGD
};


void optimize(
        std::vector<Layer>& layers,
        std::vector<Layer>& grad,
        OPTIMIZATION_ALGO algorithm,
        std::map<std::string, std::string>& algorithm_parameters,
        bool verbose
        );


void sgd(std::vector<Layer>& layers,
         std::vector<Layer>& grad,
         std::map<std::string, std::string>& parameters,
         bool verbose
);


#endif //NN_OPTIMIZATION_H