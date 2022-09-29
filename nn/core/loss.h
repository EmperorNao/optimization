#ifndef NN_LOSS_H
#define NN_LOSS_H

#include <string>
#include <cmath>
#include <algorithm>

#include "numerical/numerical.h"
#include "matrix.h"


class Loss {

private:
    std::string name;

public:
    Loss(std::string _name): name(_name) {};

    virtual double value(Matrix<double>& answers, Matrix<double>& labels) = 0;
    virtual Matrix<double> derivative(Matrix<double>& data, Matrix<double>& labels) = 0;
    virtual ~Loss() {};

};


class BCELoss: public Loss {

private:
    double clamp_log(double val, double clamp=-100000) {
        return std::max(std::log(std::max(val, 1e-8)), clamp);
    }

public:

    BCELoss(): Loss("BCELoss") {};

    double value(Matrix<double>& answers, Matrix<double>& labels);
    Matrix<double> derivative(Matrix<double>& data, Matrix<double>& labels);
    virtual ~BCELoss() {};

};


class CELossWithLogits: public Loss {

private:
    double clamp_log(double val, double clamp=-100) {
        return std::max(std::log(std::max(val, 1e-8)), clamp);
    }

public:

    CELossWithLogits(): Loss("CELossWithLogits") {};

    double value(Matrix<double>& answers, Matrix<double>& labels);
    Matrix<double> derivative(Matrix<double>& data, Matrix<double>& labels);
    virtual ~CELossWithLogits() {};

};


#endif //NN_LOSS_H
