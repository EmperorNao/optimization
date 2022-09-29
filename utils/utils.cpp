#include "utils.h"


std::vector<std::string> split(std::string& s, char delim) {

    std::vector<std::string> output;

    uint64_t start = 0;
    uint64_t current = 0;
    while (current < s.size()) {

        if (s[current] == delim) {
            output.push_back(s.substr(start, current - start));
            start = current + 1;
        }

        ++current;

    }

    if (start != current) {
        output.push_back(s.substr(start, current - start));
    }

    return output;

}


Matrix<double> shuffle_matrix(Matrix<double> matr, std::vector<uint64_t> indexes) {


    Matrix<double> out(matr.size());
    for (uint64_t idx_obj = 0; idx_obj < matr.size()[0]; ++idx_obj) {

        auto real_idx = indexes[idx_obj];
        for (uint64_t feature = 0; feature < matr.size()[1]; ++feature) {
            out[{real_idx, feature}] = matr[{idx_obj, feature}];
        }

    }

    return out;

}
