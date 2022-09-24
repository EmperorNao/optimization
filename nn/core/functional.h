#ifndef NN_CORE_FUNCTIONAL_H
#define NN_CORE_FUNCTIONAL_H

#include "matrix.h"

class Functional {

private:
    std::string name;
    uint64_t deriv_args;

public:
    Functional(std::string _name, uint64_t _deriv_args = 1): name(_name), deriv_args(_deriv_args) {};

    virtual Matrix<double> operator()(Matrix<double>& data) = 0;
    virtual Matrix<double> derivative(Matrix<double>& data) = 0;
    virtual Matrix<double> derivative2(Matrix<double>& data, Matrix<double>& labels) {
        throw std::domain_error("derivative2 not implemented for " + this->name);
    }

};


#endif //NN_FUNCTIONAL_H
