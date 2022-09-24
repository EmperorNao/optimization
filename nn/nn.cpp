#include "nn.h"


void FullyConnectedNN::layer_init(struct Layer& layer, LayerOptions options, Matrix<double>& data) {

//    if (options.initialization() == WEIGHT_INITIALIZATION::GAUSSIAN) {
//        uint64_t n = data.size()[0];
//    }
//


}


Matrix<double> FullyConnectedNN::forward(Matrix<double> data, bool verbose) {

    if (!initialized) {
        initialized = true;

        for (uint64_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx) {
            layer_init(layers[layer_idx], layers_options[layer_idx], data);
        }

    }

    Matrix<double> output(std::vector<uint64_t> {0, 0});
    return output;

}
