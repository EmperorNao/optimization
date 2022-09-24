#ifndef NN_NN_H
#define NN_NN_H

#include <vector>
#include <random>

#include "functional.h"
#include "numerical/numerical.h"
#include "layer_options.h"


class FullyConnectedNN {

private:
    struct Layer {
        Matrix<double> w;
        Matrix<double> b;
    };

    std::vector<LayerOptions> layers_options;
    bool initialized = false;
    std::vector<Layer> layers;


    void layer_init(struct Layer& layer, LayerOptions options, Matrix<double>& data);

public:
    explicit FullyConnectedNN(std::vector<LayerOptions> options): layers_options(options) {}

    Matrix<double> forward(Matrix<double> data, bool verbose = false);
    Matrix<double> backward(); // understand it

};


#endif //NN_NN_H
