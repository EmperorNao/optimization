#ifndef NN_NN_H
#define NN_NN_H

#include <vector>
#include <random>
#include <memory>

#include "layer.h"
#include "functional.h"
#include "../numerical/numerical.h"
#include "../optimization/optimization.h"

#include "utils.h"


class FullyConnectedNN {

private:

    bool initialized = false;

    std::vector<LayerOptions> layers_options;
    std::vector<Layer> layers;
    std::vector<Matrix<double>> inputs;


    Layer create_layer(LayerOptions& options, Matrix<double>& data);

public:
    explicit FullyConnectedNN(std::vector<LayerOptions> options): layers_options(options) {}

    Matrix<double> forward(Matrix<double>& data, bool eval = false, bool verbose = false);
    void backward(Matrix<double>& grad, bool verbose = false);

};


#endif //NN_NN_H
