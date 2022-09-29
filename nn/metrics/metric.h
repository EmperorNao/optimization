#ifndef NN_METRIC_H
#define NN_METRIC_H

#include "matrix.h"


class Metric {

private:
    std::string name;

public:
    Metric(std::string _name): name(_name) {};

    virtual double value(Matrix<double>& answers, Matrix<double>& labels) = 0;
    virtual ~Metric() {};

};


class Accuracy: public Metric {

public:
    Accuracy(): Metric("Accuracy") {};

    double value(Matrix<double>& answers, Matrix<double>& labels);

};


class MultiAccuracy: public Metric {

public:
    MultiAccuracy(): Metric("MultiAccuracy") {};

    double value(Matrix<double>& answers, Matrix<double>& labels);

};


#endif //NN_METRIC_H
