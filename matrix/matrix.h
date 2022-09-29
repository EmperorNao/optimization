#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H


#include <vector>
#include <iostream>
#include <numeric>


template <typename T>
class Matrix {

private:
    std::vector<uint64_t> dimensions;
    std::vector<uint64_t> dimensions_bias;
    std::vector<T> data;

    void init_bias() {

        if (!dimensions.size()) {
            return;
        }

        uint64_t mult = 1;
        for (int64_t idx = dimensions.size() - 1; idx >= 0; idx -= 1) {
            dimensions_bias[idx] = mult;
            mult *= dimensions[idx];
        }

    }

public:

    Matrix() {

        dimensions.resize(0);
        dimensions_bias.resize(0);
        data.resize(0);

    }


    Matrix(std::vector<uint64_t> dims) {
        this->resize(dims);
    }

    Matrix(std::vector<uint64_t> dims, T default_value) {
        this->resize(dims, default_value);
    }

    Matrix(uint64_t size, T default_value) {
        dimensions.resize(1, size);
        dimensions_bias.resize(dimensions.size());
        data.resize(size, default_value);
        init_bias();
    }

    Matrix(uint64_t size) {
        dimensions.resize(1, size);
        dimensions_bias.resize(dimensions.size());
        data.resize(size);
        init_bias();
    }

    T& get(std::initializer_list<uint64_t> indexes) {

        if (indexes.size() != dimensions.size()) {
            throw std::invalid_argument("Wrong number of indexes");
        }

        uint64_t bias = 0;

        uint64_t index_number = 0;
        for (auto idx: indexes) {
            bias += idx * dimensions_bias[index_number];
            index_number += 1;
        }

        return data[bias];

    }

    T& operator[](std::initializer_list<uint64_t> indexes) {
        return this->get(indexes);
    }

    void resize(std::vector<uint64_t> dims) {
        dimensions = dims;
        dimensions_bias.resize(dims.size());
        int64_t n_elements  = std::accumulate(begin(dimensions), end(dimensions), 1, std::multiplies<int>());
        data.resize(n_elements);
        init_bias();
    }

    void resize(std::vector<uint64_t> dims, T default_value) {
        dimensions = dims;
        dimensions_bias.resize(dims.size());
        int64_t n_elements  = std::accumulate(begin(dimensions), end(dimensions), 1, std::multiplies<int>());
        data.resize(n_elements, default_value);
        init_bias();
    }

    std::vector<uint64_t> size() const {
        return dimensions;
    }

};


#endif //MATRIX_MATRIX_H
