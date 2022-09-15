#ifndef ART_ARTCLUSTERING_H
#define ART_ARTCLUSTERING_H


#include <unordered_set>

#include "config.h"
#include "matrix.h"
#include "utils.h"


class ARTClustering {
private:
    RandomUniformGenerator generator;

    double rho;
    uint64_t beta;
    uint64_t N;

    void shuffle_data(Matrix<char>& data) {

        uint64_t number_of_objects = data.size()[0];

        std::vector<uint64_t> indexes(number_of_objects);
        for (uint64_t i = 0; i < number_of_objects; ++i) {
            indexes[i] = i;
        }
        generator.shuffle(indexes);

        Matrix<char> tmp(data.size());

        for (uint64_t line_idx = 0; line_idx < number_of_objects; ++line_idx) {

            uint64_t old_idx = indexes[line_idx];
            for (uint64_t col_idx = 0; col_idx < data.size()[1]; ++col_idx) {

                tmp[{line_idx, col_idx}] = data[{old_idx, col_idx}];

            }

        }

        data = tmp;

    }

    uint64_t get_cardinality(Matrix<char>& data, uint64_t object_idx) {

        uint64_t cardinality = 0;
        for (uint64_t col_idx = 0; col_idx < data.size()[1]; ++col_idx) {
            cardinality += data[{object_idx, col_idx}] == '1';
        }
        return cardinality;

    }

    Matrix<char> get_intersection(Matrix<char>& data, uint64_t object_idx, Matrix<char>& prototypes, uint64_t prototype_idx) {

        Matrix<char> intersection_object(std::vector<uint64_t>{1, data.size()[1]});
        for (uint64_t col_idx = 0; col_idx < data.size()[1]; ++col_idx) {
            intersection_object[{0,col_idx}] = (prototypes[{prototype_idx, col_idx}] == '1' && data[{object_idx, col_idx}] == '1') ? '1' : '0';
        }
        return intersection_object;

    }

    void copy_object(Matrix<char>& src, uint64_t idx_src, Matrix<char>& dst, uint64_t idx_dst) {

        for (uint64_t col_idx = 0; col_idx < src.size()[1]; ++col_idx) {
            dst[{idx_dst, col_idx}] = src[{idx_src, col_idx}];
        }

    }

    void out(std::ostream& os, Matrix<char>& data, uint64_t obj_idx, std::string delim=",") {

        for (uint64_t col_idx = 0; col_idx + 1 < data.size()[1]; ++col_idx) {
            os << data[{obj_idx, col_idx}] << delim;
        }
        if (data.size()[1]) {
            os << data[{obj_idx, data.size()[1] - 1}];
        }
        else {
            os << "<empty>";
        }

    }


    bool is_objects_alike(Matrix<char>& data, uint64_t object_idx, Matrix<char>& prototypes, uint64_t prototype_idx, bool verbose=false) {

        auto intersection_object = get_intersection(data, object_idx, prototypes, prototype_idx);

        uint64_t intersection_cardinality = get_cardinality(intersection_object, 0);
        uint64_t prototype_cardinality = get_cardinality(prototypes, prototype_idx);
        uint64_t object_cardinality = get_cardinality(data, object_idx);

        double left_value = double(intersection_cardinality) / (beta + prototype_cardinality);
        double right_value = double(object_cardinality) / (beta + intersection_object.size()[1]);
        if (verbose) {
            std::cout << "Comparing object №" << object_idx << "\n";
            out(std::cout, data, object_idx);
            std::cout << "\nwith prototype №" << prototype_idx << "\n";
            out(std::cout, prototypes, prototype_idx);
            std::cout << "\n";
            std::cout << left_value << (left_value < right_value ? " < " : " > ") << right_value << "\n";
            std::cout << (left_value < right_value ? "Objects are not alike\n" : "Objects are alike\n");
        }

        if (left_value > right_value) {

            left_value = double(intersection_cardinality) / object_cardinality;
            right_value = rho;

            if (verbose) {
                std::cout << "--------------------\n";
                std::cout << "Measure attention\n";
                std::cout << left_value << (left_value < right_value ? " < " : " > ") << right_value << "\n";
                std::cout << (left_value < right_value ? "Test was not passed\n" : "Test was passed\n");
                std::cout << "\n";
            }

            if (left_value > right_value) {
                return true;
            }

        }
        else {
            if (verbose) {
                std::cout << "\n";
            }
        }

        return false;

    }


public:
    explicit ARTClustering(config::Config& conf): generator(conf.is_exist("seed") ? conf.get<int64_t>("seed") : 0) {
        // save parameters
        rho = conf.get<double>("rho");
        if (rho > 1.0) {
            throw std::invalid_argument("Wrong argument value: rho must be between (0, 1), provided " + std::to_string(rho));
        }
        beta = conf.get<uint64_t>("beta");
        N = conf.get<uint64_t>("N");
        if (N == 0) {
            throw std::invalid_argument("Wrong argument value: N must be greater then 0");
        }

    }

    void clusterize(Matrix<char> data, bool verbose=false) {

        if (data.size().size() != 2) {
            throw std::invalid_argument("Wrong data size: expected 2 dimensions, provided " + std::to_string(data.size().size()));
        }

        std::vector<uint64_t> clusters_idx(data.size()[0], UINT32_MAX);

        Matrix<char> prototypes(std::vector<uint64_t>{this->N, data.size()[1]});
//        shuffle_data(data);

        if (verbose) {
            std::cout << "Objects list\n";
            std::cout << "Got " << data.size()[0] << " objects\n\n";
            for (uint64_t object_idx = 0; object_idx < data.size()[0]; ++object_idx) {
                std::cout << "Obj №" << object_idx << " = ";
                out(std::cout, data, object_idx);
                std::cout << "\n";
            }
            std::cout << "--------------------\n\n";
        }

        copy_object(data, 0, prototypes, 0);
        uint64_t number_of_prototypes = 1;
        bool is_converged = true;

        uint64_t iter = 0;

        while (is_converged) {

            if (verbose) {
                std::cout << "Iteration " << iter << "\n";
            }
            is_converged = true;
            for (uint64_t object_idx = 0; object_idx < data.size()[0]; ++object_idx) {

                bool is_found_alike = false;
                for (uint64_t prototype_idx = 0; prototype_idx < number_of_prototypes; ++prototype_idx) {

                    if (is_objects_alike(data, object_idx, prototypes, prototype_idx, verbose)) {

                        if (clusters_idx[object_idx] != prototype_idx) {
                            if (verbose) {
                                std::cout << "Adding object №" << object_idx << " to cluster №" << prototype_idx << "\n";
                            }
                            is_converged = false;
                            clusters_idx[object_idx] = prototype_idx;
                        }
                        else {
                            std::cout << "Object №" << object_idx << " still in cluster №" << prototype_idx << "\n";
                        }

                        auto intersection_object = get_intersection(data, object_idx, prototypes, prototype_idx);

                        copy_object(intersection_object, 0, prototypes, prototype_idx);
                        if (verbose) {
                            std::cout << "New prototype after fusion = ";
                            out(std::cout, prototypes, prototype_idx);
                            std::cout << "\n";
                        }

                        is_found_alike = true;
                        if (verbose) {
                            std::cout << "--------------------\n\n";
                        }
                        break;

                    }

                } // end prototypes cycle

                if (!is_found_alike) {

                    if (number_of_prototypes < N) {

                        if (verbose) {
                            std::cout << "Creating a new cluster for object №" << object_idx << ": cluster №" << number_of_prototypes << "\n";
                            std::cout << "--------------------\n\n";
                        }

                        is_converged = false;

                        copy_object(data, object_idx, prototypes, number_of_prototypes);

                        clusters_idx[object_idx] = number_of_prototypes;
                        ++number_of_prototypes;
                    }

                } // end if not found


            } // end objects cycle
            ++iter;

        } // end is_converged cycle

        std::cout << "\n";

        std::cout << "Prototypes list\n";
        std::cout << "Create " << number_of_prototypes << " clusters\n\n";
        for (uint64_t prototype_idx = 0; prototype_idx < number_of_prototypes; ++prototype_idx) {
            std::cout << "Proto №" << prototype_idx << " = ";
            out(std::cout, prototypes, prototype_idx);
            std::cout << "\n";
        }
        std::cout << "--------------------\n\n";

        std::cout << "Objects list\n\n";
        for (uint64_t object_idx = 0; object_idx < data.size()[0]; ++object_idx) {
            std::cout << "Obj №" << object_idx << " in cluster №" << clusters_idx[object_idx] << " = ";
            out(std::cout, data, object_idx);
            std::cout << "\n";
        }
        std::cout << "--------------------\n\n";

    } // end function clusterize

};


#endif //ART_ARTCLUSTERING_H
