#include "dataloader.h"
#include "utils.h"


enum INPUT_FORMAT format(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (s == "csv") {
        return INPUT_FORMAT::CSV;
    }
    else if (s == "matrix") {
        return INPUT_FORMAT::MATRIX;
    }
    else {
        throw std::invalid_argument("Wrong format: " + s);
    }

}


Matrix<int64_t> dataloader::read_dataset(std::filesystem::path path, INPUT_FORMAT format) {

    switch (format) {
        case INPUT_FORMAT::CSV:
            return read_csv(path);
        case INPUT_FORMAT::MATRIX:
            return read_matrix(path);
        default:
            throw std::invalid_argument("Unsupported format while reading dataset");
    }

}


Matrix<int64_t> dataloader::read_csv(std::filesystem::path path) {

        std::ifstream input(path.c_str());

        std::string line;
        uint64_t number_of_columns = 0;

        std::getline(input, line);
        auto columns = split(line, ',');
        number_of_columns = columns.size();


        std::vector<std::vector<std::string>> objects;

        uint64_t number_of_line = 0;
        while (std::getline(input, line)) {

            auto object = split(line, ',');
            if (object.size() != number_of_columns) {
                throw std::domain_error("Got wrong number of columns in line " + std::to_string(number_of_line)
                + ": " + std::to_string(object.size()) + " against " + std::to_string(number_of_columns));
            }

            objects.push_back(object);
            ++number_of_line;

        }

        Matrix<int64_t> output(std::vector<uint64_t>{objects.size(), objects[0].size()});

        for (uint64_t line_idx = 0; line_idx < output.size()[0]; ++line_idx) {

            for (uint64_t col_idx = 0; col_idx < output.size()[1]; ++col_idx) {

                output[{line_idx, col_idx}] = std::stoull(objects[line_idx][col_idx]);

            }

        }

        return output;

}


Matrix<int64_t> dataloader::read_matrix(std::filesystem::path path) {

    std::ifstream input(path.c_str());

    std::string line;

    std::getline(input, line);
    auto columns = split(line, ',');

    if (columns.size() != 2) {
        throw std::domain_error("Wrong number of dimensions in first row: expected 2, but got "
        + std::to_string(columns.size()));
    }

    uint64_t number_of_lines = std::stoull(columns[0]);
    uint64_t number_of_columns = std::stoull(columns[1]);

    Matrix<int64_t> output(std::vector<uint64_t>{number_of_lines, number_of_columns});

    for (uint64_t line_idx = 0; line_idx < output.size()[0]; ++line_idx) {

        std::getline(input, line);
        auto elements = split(line, ',');

        for (uint64_t col_idx = 0; col_idx < output.size()[1]; ++col_idx) {

            output[{line_idx, col_idx}] = std::stoull(elements[col_idx]);

        }

    }
    return output;

}