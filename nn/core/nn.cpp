#include "nn.h"


std::vector<Layer> FullyConnectedNN::compute_grad(Matrix<double>& predicted, Matrix<double>& labels) {

    std::vector<Layer> layers_grad;
    std::vector<Matrix<double>> errors(layers.size() + 1);

    auto last_layer_error = loss->derivative(predicted, labels);
    Matrix<double> activation_matrix;

    errors[layers.size()] = last_layer_error;


    for (int64_t layer_idx = layers.size() - 1; layer_idx >= 0; --layer_idx) {

        auto weights_transposed = transpose(layers[layer_idx].w);

        if (layers[layer_idx].has_activation) {
            activation_matrix = layers[layer_idx].activation->derivative(inputs[layer_idx + 1].a);
            last_layer_error = dot_product(last_layer_error, activation_matrix);
        }
        last_layer_error = matrix_product(  errors[layer_idx + 1], weights_transposed);


       errors[layer_idx] = last_layer_error;

    }

    for (uint64_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx) {

        Layer layer_grad(ACTIVATION::Identity);

        layer_grad.b = errors[layer_idx + 1];

        auto transposed_input = transpose(inputs[layer_idx].a);
        layer_grad.w = matrix_product(transposed_input, errors[layer_idx + 1]);

        layers_grad.push_back(layer_grad);
    }

    return layers_grad;

}


Layer FullyConnectedNN::create_layer(LayerOptions& options, Matrix<double>& data) {

    Layer layer(options.activation());
    uint64_t n = data.size()[0];

    layer.w.resize(std::vector<uint64_t>{options.inputs(), options.outputs()});
    layer.b.resize(std::vector<uint64_t>{1, options.outputs()});

    for (uint64_t x = 0; x < layer.b.size()[0]; ++x) {
        for (uint64_t y = 0; y < layer.b.size()[1]; ++y) {
            layer.b[{x, y}] = 0;
        }
    }

    if (options.initialization() == WEIGHT_INITIALIZATION::GAUSSIAN) {

        double stddev = std::sqrt(2.0 / n);
        auto generator = GaussianGenerator(0, 0.0, stddev);
        for (uint64_t x = 0; x < layer.w.size()[0]; ++x) {
            for (uint64_t y = 0; y < layer.w.size()[1]; ++y) {
                layer.w[{x, y}] = generator.generate();
            }
        }

    }

    return layer;
}


Matrix<double> FullyConnectedNN::forward(Matrix<double>& data, bool eval, bool verbose) {

    if (!initialized) {
        initialized = true;

        for (auto & layers_option : layers_options) {
            layers.push_back(create_layer(layers_option, data));
        }

    }

    if (!inputs.empty()) {
        throw std::domain_error("Last inputs not empty in FullyConnectedNN. "
                                "Need to call backward(...) before call forward(...) twice in a row.");
    }

    inputs.resize(layers.size() + 1);

    inputs[0].a = data;
    for (uint64_t idx = 0; idx < layers.size(); ++idx) {

        inputs[idx + 1].z = linear_operator(inputs[idx].a, layers[idx].w, layers[idx].b);
        if (layers[idx].has_activation) {
            inputs[idx + 1].a = layers[idx].activation->function(inputs[idx + 1].z);
        }
        else {
            inputs[idx + 1].a = inputs[idx].z;
        }

    }

    Matrix<double> output = inputs[layers.size()].a;

    if (eval) {
        inputs.clear();
    }

    return output;

}


double FullyConnectedNN::backward(Matrix<double>& predicted, Matrix<double>& labels, bool verbose) {

    double loss_value = loss->value(predicted, labels) / predicted.size()[0];
    std::vector<Layer> layers_grad = compute_grad(predicted, labels);

    optimize(layers, layers_grad, OPTIMIZATION_ALGO::SGD, optimization_parameters, verbose);
    inputs.clear();

    return loss_value;

}