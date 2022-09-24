#ifndef NN_CORE_FUNCTIONAL_H
#define NN_CORE_FUNCTIONAL_H

#include "matrix.h"
#include <algorithm>
#include <cmath>

#include "../numerical/numerical.h"

class Functional {

private:
    std::string name;
//    uint64_t deriv_args;

public:
    Functional(std::string _name): name(_name) {};

    virtual Matrix<double> function(Matrix<double>& data) = 0;
    virtual Matrix<double> derivative(Matrix<double>& data) = 0;
//    virtual Matrix<double> derivative2(Matrix<double>& data, Matrix<double>& labels) {
//        throw std::domain_error("derivative2 not implemented for " + this->name);
//    }
    virtual ~Functional() {};

};


enum class ACTIVATION {
    ReLU, Sigmoid, Identity
};


class ReLU: public Functional {
public:
    ReLU(): Functional("ReLU") {};
    Matrix<double> function(Matrix<double>& data);
    Matrix<double> derivative(Matrix<double>& data);
};


class Sigmoid: public Functional {

private:
    inline double sigmoid(double value) {
        return 1 / (1 + std::exp(-value));
    }

public:
    Sigmoid(): Functional("Sigmoid") {};
    Matrix<double> function(Matrix<double>& data);
    Matrix<double> derivative(Matrix<double>& data);
};


class Identity: public Functional {

public:
    Identity(): Functional("Identity") {};
    Matrix<double> function(Matrix<double>& data) { return data; }
    Matrix<double> derivative(Matrix<double>& data) { return data; }
};




#endif //NN_FUNCTIONAL_H
