#ifndef NN_LAYER_OPTIONS_H
#define NN_LAYER_OPTIONS_H

#include <cstdint>
#include <map>

#include "functional.h"


enum class WEIGHT_INITIALIZATION {
    XAVIER, GAUSSIAN
};


class LayerOptions {
    uint64_t inputs_val;
    uint64_t outputs_val;
    ACTIVATION activation_val;
    WEIGHT_INITIALIZATION initialization_val;

public:

    static const ACTIVATION DEFAULT_ACTIVATION = ACTIVATION::ReLU;
    static const WEIGHT_INITIALIZATION DEFAULT_INITIALIZATION = WEIGHT_INITIALIZATION::GAUSSIAN;


    LayerOptions(uint64_t inputs, uint64_t outputs,
                 ACTIVATION activation=DEFAULT_ACTIVATION,
                 WEIGHT_INITIALIZATION initialization=DEFAULT_INITIALIZATION
                 //,use_residual=NOT_IMPLEMENTED_YET
    ): inputs_val(inputs), outputs_val(outputs), activation_val(activation), initialization_val(initialization) {};

    uint64_t inputs() const { return inputs_val; }
    uint64_t outputs() const { return outputs_val; }
    ACTIVATION activation() const { return activation_val; }
    WEIGHT_INITIALIZATION initialization() const {return initialization_val; }

};


#endif //NN_LAYER_OPTIONS_H
