#ifndef OPTIMIZATION_UTILS_H
#define OPTIMIZATION_UTILS_H


#include <random>
#include <algorithm>
#include <iterator>
#include <vector>


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

};



#endif //OPTIMIZATION_UTILS_H
