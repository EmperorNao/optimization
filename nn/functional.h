#ifndef NN_FUNCTIONAL_H
#define NN_FUNCTIONAL_H

#include <algorithm>
#include <cmath>

#include "core/functional.h"
#include "numerical/numerical.h"


enum class ACTIVATION {
    ReLU, Sigmoid
};


class ReLU: public Functional {
public:
    ReLU(): Functional("ReLU") {};
    Matrix<double> operator()(Matrix<double>& data);
    Matrix<double> derivative(Matrix<double>& data);
};


class Sigmoid: public Functional {

private:
    inline double sigmoid(double value) {
        return 1 / (1 + std::exp(-value));
    }

public:
    Sigmoid(): Functional("Sigmoid") {};
    Matrix<double> operator()(Matrix<double>& data);
    Matrix<double> derivative(Matrix<double>& data);
};


class Softmax: public Functional {

private:
    inline double sigmoid(double value) {
        return 1 / (1 + std::exp(-value));
    }

public:
    Softmax(): Functional("Softmax") {};
    Matrix<double> operator()(Matrix<double>& data);
    Matrix<double> derivative(Matrix<double>& data);
};




#endif //NN_FUNCTIONAL_H
