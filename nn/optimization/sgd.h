#ifndef NN_SGD_H
#define NN_SGD_H

#include <string>

#include "../core/layer.h"
#include "../numerical/numerical.h"


void sgd(std::vector<Layer>& layers,
         std::vector<Matrix<double>>& inputs,
         Matrix<double>& grad,
         std::map<std::string, std::string>& parameters
         ) {

    double lr = std::stod(parameters["lr"]);

    Matrix<double> current_grad(grad);
    for (int64_t layer_idx = layers.size() - 1; layer_idx >= 0; --layer_idx) {

        if (layers[layer_idx].has_activation) {
            current_grad = layers[layer_idx].activation->derivative(grad);
        }

        auto b_step = matrix_scalar_multiplication(current_grad, -lr);
        layers[layer_idx].b = matrix_addition(layers[layer_idx].b, b_step);

        transpose(inputs[layer_idx]);
        auto w_step = matrix_product(inputs[layer_idx], b_step);
        layers[layer_idx].w = matrix_addition(layers[layer_idx].w, w_step);

    }

}


#endif //NN_SGD_H
