#ifndef GENETIC_GENETICSIMULATOR_H
#define GENETIC_GENETICSIMULATOR_H


#include <algorithm>

#include "config.h"
#include "utils.h"


class GeneticSimulator {

private:

    RandomUniformGenerator generator;

    uint64_t n;
    uint64_t population_cardinality;
    uint64_t max_iterations;

    double p_c;
    double p_m;

    struct chromosom {
        std::vector<uint64_t> sequence;
        double cost = MAXFLOAT;
    };

    double cost_function(chromosom& chr, Matrix<int64_t>& distance) {

        double cost = 0;
        for (uint64_t idx = 1; idx < chr.sequence.size(); ++idx) {
            auto i = chr.sequence[idx - 1];
            auto j = chr.sequence[idx];
            cost += distance[{i, j}];
        }
        cost += distance[{chr.sequence[chr.sequence.size() - 1], chr.sequence[0]}];

        return cost;

    }

    std::vector<chromosom> create_population () {

        std::vector<chromosom> population(population_cardinality);
        for (uint64_t idx = 0; idx < population_cardinality; ++idx) {
            population[idx].sequence = generator.random_permutation(n);
        }
        return population;

    }

    void compute_population_cost(std::vector<chromosom>& population, Matrix<int64_t>& distance) {

        for (auto& chr: population) {
            chr.cost = cost_function(chr, distance);
        }

    }

    void do_probability(std::vector<double>& costs) {

        double sum = 0;
        for (auto& obj: costs) {
            sum += obj;
        }

        for (auto& obj: costs) {
            obj /= sum;
        }

    }

    std::vector<chromosom> selection(std::vector<chromosom>& chromosoms) {

        std::vector<chromosom> selected(chromosoms.size());

        std::vector<double> costs(chromosoms.size());
        for (uint64_t idx = 0; idx < chromosoms.size(); ++idx) {
            costs[idx] = 1.0 / chromosoms[idx].cost;
        }
        do_probability(costs);

        for (uint64_t idx = 0; idx < chromosoms.size(); ++idx) {
            selected[idx] = chromosoms[roulette(costs)];
        }

        return selected;

    }

    uint64_t roulette(std::vector<double>& proba) {

        double generated_proba = generator.uniform_int(0, 99) / 100.0;

        uint64_t selected = 0;
        double proba_sum = 0;
        for (uint64_t idx = 0; idx < proba.size(); ++idx) {
            proba_sum += proba[idx];
            if (proba[idx] != 0 && generated_proba <= proba_sum) {
                selected = idx;
                break;
            }
        }

        return selected;

    }

    chromosom cross(chromosom& left, chromosom& right) {

        std::vector<uint64_t> gens;
        for (uint64_t idx = 0; idx < left.sequence.size(); ++idx) {
            double proba = generator.uniform_int(0, 100) / 100.0;
            if (proba < p_c) {
                gens.push_back(left.sequence[idx]);
            }
        }

        chromosom out {
            .sequence = std::vector<uint64_t> (left.sequence.size())
        };

        uint64_t ptr = 0;
        for (uint64_t idx = 0; idx < left.sequence.size(); ++idx) {
            if (std::find(gens.begin(), gens.end(), right.sequence[idx]) != gens.end()) {
                out.sequence[idx] = right.sequence[idx];
            }
            else {
                while (std::find(gens.begin(), gens.end(), left.sequence[ptr]) != gens.end()) {
                    ++ptr;
                }
                out.sequence[idx] = left.sequence[ptr];
                ++ptr;
            }
        }

        return out;

    }

    chromosom mutate(chromosom& chr) {

        chromosom out {
            .sequence = chr.sequence
        };


        double proba = generator.uniform_int(0, 100) / 100.0;
        if (proba < p_m) {

            int64_t q_idx1 = generator.uniform_int(0, chr.sequence.size() - 1);

            int64_t q_idx2 = generator.uniform_int(0, chr.sequence.size() - 2);
            q_idx2 = q_idx2 < q_idx1 ? q_idx2 : q_idx2 + 1;

            uint64_t q1 = chr.sequence[q_idx1];
            uint64_t q2 = chr.sequence[q_idx2];

            chr.sequence[q_idx1] = q2;
            chr.sequence[q_idx2] = q1;

        }
        return out;

    }


    std::vector<chromosom> crossingover(std::vector<chromosom>& chromosoms) {

        std::vector<chromosom> results(chromosoms.size());

        auto permutation = generator.random_permutation(chromosoms.size());
        for (uint64_t idx = 0; idx < chromosoms.size() / 2; ++idx) {
            uint64_t idx_left = permutation[idx];
            uint64_t idx_right = permutation[chromosoms.size() - idx];

            results[idx * 2] = cross(chromosoms[idx_left], chromosoms[idx_right]);
            results[idx * 2 + 1] = chromosoms[idx_left];

        }

        return results;

    }

    std::vector<chromosom> mutation(std::vector<chromosom>& chromosoms) {

        std::vector<chromosom> results(chromosoms.size());
        for (uint64_t idx = 0; idx < chromosoms.size(); ++idx) {
            results[idx] = mutate(chromosoms[idx]);

        }
        return results;

    }


public:
    explicit GeneticSimulator(config::Config conf): generator(conf.is_exist("seed") ? conf.get<int64_t>("seed") : 0) {

        n = conf.get<uint64_t>("n");
        max_iterations = conf.get<uint64_t>("max_iterations");
        population_cardinality = conf.get<uint64_t>("population_cardinality");

        p_c = conf.get<double>("p_c");
        p_m = conf.get<double>("p_m");


    }

    void simulate(Matrix<int64_t>& distance_matrix, bool verbose=false) {

        chromosom best;

        if (verbose) {
            std::cout << "Starting solving\n";
        }

        std::vector<chromosom> population = create_population();
        for (uint64_t iteration = 0; iteration < max_iterations; ++iteration) {

            if (verbose) {
                std::cout << "Iteration " << iteration << std::endl;
            }

            compute_population_cost(population, distance_matrix);

            double avg_cost = 0;
            for (auto& obj: population) {
                avg_cost += obj.cost;
                if (obj.cost < best.cost) {
                    best = obj;
                }
            }

            if (verbose) {
                std::cout << "Avg cost: " << avg_cost / population.size() << std::endl << std::endl;
            }

            if (verbose) {
                std::cout << "Best solution\n";
                for (auto &gen: best.sequence) {
                    std::cout << gen << " ";
                }
                std::cout << std::endl;
                std::cout << "Best cost: " << best.cost << std::endl;
                std::cout << std::endl << std::endl;
            }

            auto parents = selection(population);

            parents = crossingover(parents);
            population = mutation(parents);

        }

        std::cout << "Ended solving\n";
        std::cout << "Best cost: " << best.cost << std::endl;
        std::cout << "Best solution\n";
        for (auto& gen:best.sequence) {
            std::cout << gen << " ";
        }
        std::cout << std::endl << std::endl;


    }

};


#endif //GENETIC_GENETICSIMULATOR_H
