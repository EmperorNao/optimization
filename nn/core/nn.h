#ifndef NN_NN_H
#define NN_NN_H

#include <vector>
#include <random>
#include <memory>
#include <map>

#include "loss.h"
#include "layer.h"
#include "functional.h"
#include "numerical/numerical.h"
#include "optimization/optimization.h"

#include "utils.h"


class FullyConnectedNN {

private:
    struct neuron {
        Matrix<double> z;
        Matrix<double> a;
    };

    bool initialized = false;

    std::vector<LayerOptions> layers_options;
    std::vector<Layer> layers;
    std::vector<neuron> inputs;
    std::map<std::string, std::string> optimization_parameters;
    std::shared_ptr<Loss> loss;

    Layer create_layer(LayerOptions& options, Matrix<double>& data);
    std::vector<Layer> compute_grad(Matrix<double>& predicted, Matrix<double>& labels);

public:
    explicit FullyConnectedNN(std::vector<LayerOptions> options,
                              std::shared_ptr<Loss> loss,
                              std::map<std::string, std::string> optim_params):
                              layers_options(options),
                              optimization_parameters(optim_params),
                              loss(loss)
                              {}

    Matrix<double> forward(Matrix<double>& data, bool eval = false, bool verbose = false);
    double backward(Matrix<double>& predicted, Matrix<double>& labels, bool verbose = false);


};


#endif //NN_NN_H
