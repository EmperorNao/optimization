#ifndef ANTS_ANTSSIMULATOR_H
#define ANTS_ANTSSIMULATOR_H


#include <set>
#include <cmath>

#include "config.h"
#include "utils.h"
#include "matrix.h"


class AntsSimulator {

private:
    RandomUniformGenerator generator;
    double alpha;
    double beta;
    double rho;
    double Q;
    double start_pheromone;
    uint64_t k;
    uint64_t max_iterations;

public:
    explicit AntsSimulator(config::Config& conf): generator(conf.is_exist("seed") ? conf.get<int64_t>("seed") : 0) {

        alpha = conf.get<double>("alpha");
        beta = conf.get<double>("beta");
        rho = conf.get<double>("rho");
        Q = conf.get<double>("Q");
        k = conf.get<uint64_t>("k");
        start_pheromone = conf.get<double>("start_pheromone");
        max_iterations = conf.get<uint64_t>("max_iterations");
        // TODO constraints

    }

    void simulate(Matrix<int64_t> distance_matrix, bool verbose=false) {

        if (distance_matrix.size()[0] != distance_matrix.size()[1]) {
            throw std::invalid_argument("Expected for data to be a square matrix, got ("
            + std::to_string(distance_matrix.size()[0]) + " x "
            + std::to_string(distance_matrix.size()[1]) + ")");
        }

        uint64_t n_vertexes = distance_matrix.size()[0];

        std::vector<double> ants_cost(k);
        std::vector<std::vector<uint64_t>> ants_path(k);

        Matrix<std::vector<uint64_t>> vertexes_visited_ants_idx(distance_matrix.size());

        Matrix<double> pheromone_matrix(distance_matrix.size());
        for (uint64_t i = 0; i < n_vertexes; ++i) {
            for (uint64_t j = 0; j < n_vertexes; ++j) {
                pheromone_matrix[{i, j}] = start_pheromone;
            }
        }

        double best_cost = MAXFLOAT;
        std::vector<uint64_t> best_path;

        uint64_t iterations;
        // TODO another convergence condition
        for (iterations = 0; iterations < max_iterations; ++iterations) {

            for (uint64_t x = 0; x < n_vertexes; ++x) {
                for (uint64_t y = 0; y < n_vertexes; ++y) {
                    vertexes_visited_ants_idx[{x ,y}].resize(0);
                }
            }

            for (uint64_t ant_idx = 0; ant_idx < k; ++ant_idx) {

                std::set<uint64_t> visited;
                std::vector<uint64_t> path;
                std::vector<double> proba(n_vertexes, 0);
                uint64_t current_vertex_idx = generator.uniform_int(0, n_vertexes - 1);
                visited.insert(current_vertex_idx);
                path.push_back(current_vertex_idx);
                double cost = 0;

                if (verbose) {
                    std::cout << "Starting from vertex " << current_vertex_idx << "\n";
                }

                while (path.size() != n_vertexes) {

                    if (verbose) {
                        std::cout << "Current path: \n";
                        for (auto& el: path) {
                            std::cout << el << " ";
                        }
                        std::cout << "\n";
                    }

                    // count proba
                    double proba_sum = 0;
                    for (uint64_t vertex_idx = 0; vertex_idx < n_vertexes; ++vertex_idx) {
                        if (!visited.contains(vertex_idx)) {
                            proba[vertex_idx] = std::pow(pheromone_matrix[{current_vertex_idx, vertex_idx}], alpha) *
                                                std::pow(1.0 / distance_matrix[{current_vertex_idx, vertex_idx}], beta);
                            proba_sum += proba[vertex_idx];
                        }
                        else {
                            proba[vertex_idx] = 0;
                        }
                    }

                    for (uint64_t vertex_idx = 0; vertex_idx < n_vertexes; ++vertex_idx) {
                        proba[vertex_idx] /= proba_sum;
                    }

                    if (verbose) {
                        std::cout << "Counter proba: \n";
                        uint64_t idx = 0;
                        for (auto& proba_el: proba) {
                            std::cout << "#"<< idx << " = " << proba_el << "\t";
                            ++idx;
                        }
                        std::cout << "\n";
                    }

                    double generated_proba = generator.uniform_int(0, 99) / 100.0;

                    if (verbose) {
                        std::cout << "generated probability = " << generated_proba << "\n";
                    }

                    uint64_t next_vertex_idx = 0;
                    // random choose from
                    proba_sum = 0;
                    for (uint64_t idx = 0; idx < n_vertexes; ++idx) {
                        proba_sum += proba[idx];
                        if (proba[idx] != 0 && generated_proba <= proba_sum /*or idx == n_vertexes - 1*/) {
                            next_vertex_idx = idx;
                            break;
                        }
                    }
//                    if (next_vertex_idx == 0 && generated_proba > proba_sum) {
//                        uint64_t last_idx = n_vertexes - 1;
//                        while (proba[last_idx] == 0 && last_idx > 0) {
//                            --last_idx;
//                        }
//                        next_vertex_idx = last_idx;
//                    }

                    if (verbose) {
                        std::cout << "Proba sum = " << proba_sum << "\n";
                        std::cout << "Selected vertex #" << next_vertex_idx << "\n";
                    }

                    cost += distance_matrix[{current_vertex_idx, next_vertex_idx}];
                    visited.insert(next_vertex_idx);
                    path.push_back(next_vertex_idx);

                    vertexes_visited_ants_idx[{current_vertex_idx, next_vertex_idx}].push_back(ant_idx);

                    current_vertex_idx = next_vertex_idx;

                }

                cost += distance_matrix[{current_vertex_idx, path[0]}];
                path.push_back(path[0]);
                if (verbose) {
                    std::cout << "Current path: \n";
                    for (auto& el: path) {
                        std::cout << el << " ";
                    }
                    std::cout << "\n";
                    std::cout << "Selected vertex #" << path[0] << "\n";
                    std::cout << "\n";
                }

                vertexes_visited_ants_idx[{current_vertex_idx, path[0]}].push_back(ant_idx);

                ants_cost[ant_idx] = cost;
                ants_path[ant_idx] = path;

                if (cost < best_cost) {
                    best_cost = cost;
                    best_path = path;
                }

            }

            for (uint64_t x = 0; x < n_vertexes; ++x) {
                for (uint64_t y = 0; y < n_vertexes; ++y) {

                    double sum = 0;
                    for (auto& ant_idx: vertexes_visited_ants_idx[{x, y}]) {
                        sum += Q / ants_cost[ant_idx];
                    }
                    pheromone_matrix[{x, y}] = pheromone_matrix[{x, y}] * rho + sum;

                }
            }

        }

        std::cout << "\nEnded\n";

        std::cout << "Pheromone matrix\n";
        for (uint64_t x = 0; x < n_vertexes; ++x) {
            for (uint64_t y = 0; y < n_vertexes; ++y) {

                std::cout << pheromone_matrix[{x, y}] << ", ";

            }
            std::cout << "\n";
        }
        std::cout << "\n";

        std::cout << "Best cost = " << best_cost << "\n";
        std::cout << "Best path: \n";
        for (auto& vertex: best_path) {
            std::cout << vertex << " ";
        }
        std::cout << "\n";

    } // end of simulation function
    
};


#endif //ANTS_ANTSSIMULATOR_H
