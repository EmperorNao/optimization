#ifndef OPTIMIZATION_QUEENSANNEALINGSOLVER_H
#define OPTIMIZATION_QUEENSANNEALINGSOLVER_H


#include <cmath>

#include "../config/config.h"
#include "objects.h"
#include "utils.h"


namespace annealing {

    class QueensAnnealingSolver {

    private:
        config::Config solution_config;
        std::string solution_config_file;
        RandomUniformGenerator generator;

        std::string decreasing_rule;
        std::string energy_difference_rule;
        double decreasing_coefficient;

        uint64_t max_iterations;
        uint64_t fixed_temperature_iterations;

        double min_temperature;
        double start_temperature;
        double precision;

        double probability(double temperature, double cost_diff) {

            return std::exp(-1 * cost_diff / temperature);

        }

        double decrease_temperature(double temperature) {

            if (decreasing_rule == "exponential") {
                return temperature * decreasing_coefficient;
            }
            else {
                throw std::invalid_argument("There is no such decreasing rule");
            }

        }

        double count_probability(double temperature, double old_cost, double new_cost) {

            double diff = 0;
            if (energy_difference_rule == "simple") {
                diff = new_cost - old_cost;
            }
            else if (energy_difference_rule == "norm_max") {
                diff = (new_cost - old_cost) / (new_cost + precision);
            }
            else if (energy_difference_rule == "norm_min") {
                diff = (new_cost - old_cost) / (old_cost + precision);
            }
            else {
                throw std::invalid_argument("There is no such decreasing rule");
            }

            auto prob = probability(temperature, diff);
            prob = prob <= 1.0 ? prob : 1.0;

            return prob;

        }

        void print_options(std::ostream& os) {

            os << "Reading solution config from " << solution_config_file << std::endl;
            os << "n = " << solution_config.get<uint64_t>("n") << std::endl;

            if (solution_config.is_exist("seed")) {
                os << "seed = " << solution_config.get<int64_t>("seed") << std::endl;
            }
            else {
                os << "seed = 0 [Using default value]" << std::endl;
            }

            if (solution_config.is_exist("start_solution")) {
                os << "start_solution = " << solution_config.get<int64_t>("start_solution") << std::endl;
            }
            else {
                os << "Don't find start_solution, starting with random permutation" << std::endl;
            }

            os << "Using " << decreasing_rule << " decreasing rule with parameters:" << std::endl;
            if (decreasing_rule == "exponential") {
                os << "\tdecreasing_coefficient = " << decreasing_coefficient << std::endl;
            }

            os << "Using " << energy_difference_rule << " energy difference rule" << std::endl;

            os << "Main parameters for annealing:" << std::endl;
            os << "\tMax number of outer iterations = " << max_iterations << std::endl;
            os << "\tNumber of inner iterations = " << fixed_temperature_iterations << std::endl;
            os << "\tStart with temperature = " << start_temperature << std::endl;
            os << "\tMin temperature = " << min_temperature << std::endl;
            os << "\tUsing precision = " << precision << std::endl;

        }

    public:

        explicit QueensAnnealingSolver(const config::Config& conf):
        generator(conf.is_exist("seed") ? conf.get<int64_t>("seed") : 0)
        {

            solution_config_file = conf.get<std::string>("solution_config_file");

            max_iterations = conf.get<uint64_t>("max_iterations", 10000);
            start_temperature = conf.get<double>("start_temperature", 30);
            min_temperature = conf.get<double>("min_temperature", 0.05);
            fixed_temperature_iterations = conf.get<uint64_t>("fixed_temperature_iterations", 1000);
            precision = conf.get<double>("precision", 1e-6);

            decreasing_rule = conf.get<std::string>("decreasing_rule", "exponential");
            energy_difference_rule = conf.get<std::string>("energy_difference_rule", "simple");
            decreasing_coefficient = conf.get<double>("decreasing_coefficient", 0.98);

        }

        QueensSolution solve(bool verbose) {

            solution_config.load(solution_config_file);

            QueensSolution current_solution(solution_config, generator);
            QueensSolution best_solution = current_solution;

            double current_temperature = start_temperature;

            if (verbose) {
                print_options(std::cout);
            }
            std::cout << "Starting annealing" << std::endl;

            uint64_t iteration = 0;
            for (iteration = 0; (iteration < max_iterations) and (current_temperature > min_temperature + precision); ++iteration) {

                if (verbose) {
                    std::cout << "iter " << iteration << std::endl;
                    std::cout << "best solution = " << best_solution << std::endl;
                    std::cout << "best cost = " << best_solution.cost() << std::endl;
                    std::cout << "current temperature = " << current_temperature << std::endl;
                }

                for (uint64_t inner_iteration = 0; inner_iteration < fixed_temperature_iterations; ++inner_iteration) {

                    if (verbose) {
                        std::cout << "inner iter " << inner_iteration << std::endl;
                        std::cout << "current solution = " << current_solution << std::endl;
                        std::cout << "current cost = " << current_solution.cost() << std::endl;
                    }

                    auto step_solution = current_solution;
                    step_solution.random_step(generator);

                    auto current_cost = current_solution.cost();
                    auto step_cost = step_solution.cost();
                    auto prob = count_probability(current_temperature, current_cost, step_cost);

                    if (verbose) {
                        std::cout << "random step with new solution = " << current_solution << std::endl;
                        std::cout << "new cost = " << current_solution.cost() << std::endl;
                        std::cout << "probability = " << prob << std::endl;
                    }

                    if (step_cost < current_cost) {
                        current_solution = step_solution;

                        if (step_cost < best_solution.cost()) {
                            best_solution = step_solution;
                        }

                    }
                    else if (generator.uniform_int(0, 100) < prob * 100) {
                        current_solution = step_solution;
                    }

                    if (verbose) {
                        std::cout << std::endl;
                    }

                }
                current_solution = best_solution;
                current_temperature = decrease_temperature(current_temperature);

                if (verbose) {
                    std::cout << std::endl;
                }

            }

            std::cout << "Ended search on " << iteration << " iteration" << std::endl;
            std::cout << "Found best solution = " << best_solution << std::endl;
            std::cout << "Best cost = " << best_solution.cost() << std::endl;

            return best_solution;

        };

    };

}


#endif //OPTIMIZATION_QUEENSANNEALINGSOLVER_H