#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>

#include "matrix.h"



std::vector<std::string> split(std::string& s, char delim=',');


class RandomUniformGenerator {

private:
    std::mt19937 generator;

public:
    RandomUniformGenerator(int64_t seed) {
        generator.seed(seed);
    }

    int64_t uniform_int(int64_t left, int64_t right) {

        std::uniform_int_distribution<int64_t> distrib(left, right);
        return distrib(generator);

    }

    std::vector<int64_t> uniform_int(int64_t left, int64_t right, uint64_t size) {

        std::uniform_int_distribution<int64_t> distrib(left, right);

        std::vector<int64_t> out(size);
        for (uint64_t i = 0; i < size; ++i) {
            out[i] = distrib(generator);
        }

        return out;

    }

    void shuffle(std::vector<uint64_t>& vec) {

        std::shuffle(vec.begin(), vec.end(), generator);

    }

    std::vector<uint64_t> random_permutation(uint64_t n) {

        std::vector<uint64_t> obj(n);
        for (uint64_t i = 0; i < n; ++i) {
            obj[i] = i;
        }
        shuffle(obj);
        return obj;

    }


};


class GaussianGenerator {

private:
    std::mt19937 generator;
    std::normal_distribution<double> distribution;

public:
    GaussianGenerator(int64_t seed, double mean, double stddev = 1.0): distribution(mean, stddev) {
        generator.seed(seed);
    }

    double generate() {

        return distribution(generator);

    }


    void shuffle(std::vector<uint64_t>& vec) {

        std::shuffle(vec.begin(), vec.end(), generator);

    }

};


Matrix<double> shuffle_matrix(Matrix<double> matr, std::vector<uint64_t> indexes);



#endif //UTILS_UTILS_H