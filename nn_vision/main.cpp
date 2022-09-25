#include <iostream>
#include <memory>

#include "../nn/core/nn.h"
#include "../nn/core/loss.h"
#include "../nn/metrics/metric.h"


int main() {

    std::map<std::string, std::string> params;
    params["lr"] = "0.01";
    uint64_t n_objects = 1000;
    uint64_t n_features = 2;
    GaussianGenerator gen(0, 0, 5);
    RandomUniformGenerator idx_gen(0);
    auto indexes = idx_gen.random_permutation(2 * n_objects);

    Matrix<double> data(std::vector<uint64_t>{2 * n_objects, n_features});
    Matrix<double> answers(std::vector<uint64_t>{2 * n_objects, 1});

    for (uint64_t class_type = 0; class_type < 2; ++class_type) {

        for (uint64_t idx = 0; idx < n_objects; ++idx) {

            data[{idx + n_objects * class_type, 0}] = 50 * class_type + gen.generate();
            data[{idx + n_objects * class_type, 1}] = 50 * class_type + gen.generate();
            answers[{idx + n_objects * class_type, 0}] = class_type;

        }

    }

    data = shuffle_matrix(data, indexes);
    answers = shuffle_matrix(answers, indexes);

    auto loss = std::make_shared<BCELoss>();

    FullyConnectedNN network(
            {
//                LayerOptions(n_features, 3),
//                LayerOptions(3, 5),
                LayerOptions(n_features, 1, ACTIVATION::Sigmoid)
            },
            static_cast<std::shared_ptr<Loss>>(loss),
            params
            );


    for (uint64_t epoch = 0; epoch < 1000; ++epoch) {

        auto predicted = network.forward(data);
        double loss_val = network.backward(predicted, answers);

        auto predicted_labels = map(predicted,
                                    [](double val) { return val > 0.5 ? 1.0 : 0.0; }
        );

        Accuracy acc;
        auto accuracy_val = acc.value(predicted_labels, answers);

        std::cout << epoch << " = epoch\n" << loss_val << " = loss\n" << accuracy_val << " = accuracy\n";
        std::cout << "\n";

    }


}
