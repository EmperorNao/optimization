#ifndef ART_DATALOADER_H
#define ART_DATALOADER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ranges>
#include <string_view>

#include "matrix.h"



 enum class INPUT_FORMAT {
    CSV, MATRIX
};

enum INPUT_FORMAT format(std::string s);


namespace dataloader {


    Matrix<int64_t> read_csv(std::filesystem::path path);
    Matrix<int64_t> read_matrix(std::filesystem::path path);


    Matrix<int64_t> read_dataset(std::filesystem::path path, INPUT_FORMAT format);

};


#endif //ART_DATALOADER_H
