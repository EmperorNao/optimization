#include "optimization.h"


void sgd(std::vector<Layer>& layers,
         std::vector<Layer>& grad,
         std::map<std::string, std::string>& parameters,
         bool verbose
) {

    double lr = std::stod(parameters["lr"]);
    for (uint64_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx ) {

        grad[layer_idx].b = map(grad[layer_idx].b, [](double x) { return std::clamp(x, -100000.0, 100000.0);});
        auto b_step = matrix_scalar_multiplication(grad[layer_idx].b, -lr);
        b_step = mean(b_step, 1);
        layers[layer_idx].b = matrix_addition(layers[layer_idx].b, b_step);

        grad[layer_idx].w = map(grad[layer_idx].w, [](double x) { return std::clamp(x, -100000.0, 100000.0);});
        auto w_step = matrix_scalar_multiplication(grad[layer_idx].w, -lr);
        layers[layer_idx].w = matrix_addition(layers[layer_idx].w, w_step);

    }

}