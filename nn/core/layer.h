#ifndef NN_LAYER_H
#define NN_LAYER_H

#include <cstdint>
#include <map>
#include <memory>

#include "core/functional.h"


class Layer {

public:
    Matrix<double> w;
    Matrix<double> b;

    double mean;
    double var;
    bool use_batch_norm;


    std::shared_ptr<Functional> activation;
    bool has_activation = true;
    Layer(ACTIVATION _activation, bool use_bn = false): use_batch_norm(use_bn) {
        if (_activation == ACTIVATION::ReLU) {
            activation = std::make_shared<ReLU>();
        }
        else if (_activation == ACTIVATION::Sigmoid) {
            activation = std::make_shared<Sigmoid>();
        }
        else if (_activation == ACTIVATION::Identity) {
            activation = std::make_shared<Identity>();
            has_activation = false;
        }
    }

};


enum class WEIGHT_INITIALIZATION {
    XAVIER, GAUSSIAN
};


class LayerOptions {
    uint64_t inputs_val;
    uint64_t outputs_val;
    bool use_batch_norm_val;
    ACTIVATION activation_val;
    WEIGHT_INITIALIZATION initialization_val;

public:

    static const ACTIVATION DEFAULT_ACTIVATION = ACTIVATION::ReLU;
    static const WEIGHT_INITIALIZATION DEFAULT_INITIALIZATION = WEIGHT_INITIALIZATION::GAUSSIAN;
    static const bool DEFAULT_USE_BATCH_NORM = false;


    LayerOptions(uint64_t inputs, uint64_t outputs,
                 ACTIVATION activation=DEFAULT_ACTIVATION,
                 WEIGHT_INITIALIZATION initialization=DEFAULT_INITIALIZATION,
                 bool use_bn=DEFAULT_USE_BATCH_NORM
                 //,use_residual=NOT_IMPLEMENTED_YET
    ):
    inputs_val(inputs),
    outputs_val(outputs),
    use_batch_norm_val(use_bn),
    activation_val(activation),
    initialization_val(initialization)
    {};

    uint64_t inputs() const { return inputs_val; }
    uint64_t outputs() const { return outputs_val; }
    ACTIVATION activation() const { return activation_val; }
    WEIGHT_INITIALIZATION initialization() const {return initialization_val; }
    bool use_batch_norm() const { return use_batch_norm_val; }

};


class BatchNorm {
public:
    BatchNorm() {}
    Matrix<double> function(Matrix<double>& data, double mean, double var);
    Layer derivative(Matrix<double>& data, double mean, double gamma);

};


#endif //NN_LAYER_H
