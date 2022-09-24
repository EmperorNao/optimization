#include "nn.h"


Layer FullyConnectedNN::create_layer(LayerOptions& options, Matrix<double>& data) {

    Layer layer = Layer(options.activation());
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
    inputs[0] = data;

    for (uint64_t idx = 0; idx < layers.size(); ++idx) {

        inputs[idx + 1] = linear_operator(inputs[idx], layers[idx].w, layers[idx].b);
        if (layers[idx].has_activation) {
            inputs[idx + 1] = layers[idx].activation->function(inputs[idx + 1]);
        }

    }

    Matrix<double> output = inputs.back();
    inputs.pop_back();

    if (eval) {
        inputs.clear();
    }

    return output;

}


void backward(Matrix<double>& grad, bool verbose) {




}