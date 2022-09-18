#include <iostream>
#include <string>
#include <filesystem>

#include "../config/config.h"
#include "../argparse/include/argparse/argparse.hpp"
#include "dataloader.h"
#include "matrix.h"
#include "antssimulator.h"


int main(int argc, char** argv) {

    try {
        argparse::ArgumentParser program("commivoyager ants solver");
        program.add_argument("--conf")
                .required()
                .help("file with settings for solving");

        program.add_argument("--verbose")
                .help("increase output verbosity")
                .default_value(false)
                .implicit_value(true);


        program.parse_args(argc, argv);

        config::Config conf;
        conf.load(program.get("--conf"));

        std::filesystem::path data_path(conf.get<std::string>("data_path"));

        auto matrix = dataloader::read_dataset(data_path, INPUT_FORMAT::MATRIX);

        AntsSimulator simulator(conf);

        // TODO convert int to uint
        simulator.simulate(matrix, program.get<bool>("--verbose"));

    }
    catch (std::exception& e) {
        std::cout << "Failed with error: " << e.what() << std::endl;
    }

}
