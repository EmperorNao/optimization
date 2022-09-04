#ifndef OPTIMIZATION_OBJECTS_H
#define OPTIMIZATION_OBJECTS_H


#include <functional>
#include <memory>
#include <map>
#include <vector>

#include "../config/config.h"
#include "utils.h"


namespace annealing {

    class QueensSolution {

    private:
        uint n;
        std::vector<uint64_t> positions;
        int64_t current_cost = -1;

        void set_solution(std::string solution) {

            uint64_t col_idx = 0;
            uint64_t cur_idx = 0;
            uint64_t start_idx = 0;
            while (cur_idx < solution.size()) {

                if (solution[cur_idx] == ' ') {
                    positions[col_idx] = std::stoi(solution.substr(start_idx, cur_idx));
                    col_idx += 1;
                    start_idx = cur_idx + 1;
                }
                cur_idx += 1;

            }
            if (cur_idx != start_idx) {
                positions[col_idx] = std::stoi(solution.substr(start_idx, cur_idx));
            }

        }

    public:
        QueensSolution(const config::Config& conf, RandomUniformGenerator& generator) {

            this->n = conf.get<uint>("n");
            positions.resize(n);

            if (conf.is_exist("start_solution")) {

                std::string start = conf.get<std::string>("start_solution");
                set_solution(start);

            }
            else {
                initialize(generator);
            }

        }

        void initialize(RandomUniformGenerator& generator) {

            for (uint64_t idx = 0; idx < n; ++idx) {
                positions[idx] = idx;
            }
            generator.shuffle(positions);

        }

        void random_step(RandomUniformGenerator& generator) {

            int64_t q_idx1 = generator.uniform_int(0, n - 1);

            int64_t q_idx2 = generator.uniform_int(0, n - 2);
            q_idx2 = q_idx2 < q_idx1 ? q_idx2 : q_idx2 + 1;

            uint64_t q1 = positions[q_idx1];
            uint64_t q2 = positions[q_idx2];

            positions[q_idx1] = q2;
            positions[q_idx2] = q1;

            current_cost = -1;

        }

        int64_t cost() {

            if (current_cost >= 0) {
                return current_cost;
            }

            int64_t cost = 0;
            for (uint64_t idx1 = 0; idx1 < positions.size(); ++idx1) {

                for (uint64_t idx2 = idx1 + 1; idx2 < positions.size(); ++idx2) {

                    if (positions[idx1] == positions[idx2]) {
                        cost += 1;
                    }
                    else if (abs(positions[idx1] - positions[idx2]) == abs(idx1 - idx2)) {
                        cost += 1;
                    }

                }

            }

            current_cost = cost;
            return cost;

        }

        friend std::ostream& operator<<(std::ostream& os, const QueensSolution& sol) {

            for (auto el: sol.positions) {
                os << el << " ";
            }
            return os;

        }

    };

}


#endif //OPTIMIZATION_OBJECTS_H