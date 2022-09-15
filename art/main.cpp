#include <iostream>
#include <string>
#include <filesystem>

#include "../config/config.h"
#include "../argparse/include/argparse/argparse.hpp"
#include "dataloader.h"

#include "artclustering.h"


int main(int argc, char** argv) {

    try {
        argparse::ArgumentParser program("art clustering");
        program.add_argument("--conf")
                .required()
                .help("file with settings for clustering");

        program.add_argument("--verbose")
                .help("increase output verbosity")
                .default_value(false)
                .implicit_value(true);


        program.parse_args(argc, argv);

        config::Config conf;
        conf.load(program.get("--conf"));

        std::filesystem::path data_path(conf.get<std::string>("data_path"));
        INPUT_FORMAT data_format = format(conf.get<std::string>("data_format"));

        auto matrix = dataloader::read_dataset(data_path, data_format);
        // convert matrix in new size
        Matrix<char> bool_matrix(matrix.size());
        for (uint64_t line_idx = 0; line_idx < matrix.size()[0]; ++line_idx) {
            for (uint64_t col_idx = 0; col_idx < matrix.size()[1]; ++col_idx) {
                bool_matrix[{line_idx, col_idx}] = matrix[{line_idx, col_idx}] ? '1': '0';
            }
        }

        ARTClustering solver(conf);

        solver.clusterize(bool_matrix, program.get<bool>("verbose"));


    }
    catch (std::exception& e) {
        std::cout << "Failed with error: " << e.what() << std::endl;
    }

}
