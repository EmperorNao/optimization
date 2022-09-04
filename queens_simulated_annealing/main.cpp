#include <iostream>

#include "../config/config.h"
#include "../argparse/argparse.hpp"
#include "QueensAnnealingSolver.h"


int main(int argc, char *argv[]) {

    try {
        argparse::ArgumentParser program("queens_simulated_annealing");
        program.add_argument("--conf")
                .required()
                .help("file with settings for annealing");

        program.add_argument("--verbose")
                .help("increase output verbosity")
                .default_value(false)
                .implicit_value(true);


        program.parse_args(argc, argv);

        config::Config conf;
        conf.load(program.get("--conf"));


        annealing::QueensAnnealingSolver solver(conf);
        auto solution = solver.solve(program.get<bool>("--verbose"));
    }
    catch (std::exception& e) {
        std::cout << "Failed with error: " << e.what() << std::endl;
    }

    return 0;

}
