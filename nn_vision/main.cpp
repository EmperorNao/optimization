#include "nn_test.h"
#include "dataloader.h"


int main() {


    auto data = dataloader::read_dataset("/home/emperornao/projects/optimization/nn_vision/test.csv", INPUT_FORMAT::CSV);
    std::map<std::string, std::string> params;
    params["lr"] = "1e-1";


    uint64_t n_objects = data.size()[0];
    uint64_t n_features = data.size()[1] - 1;
    uint64_t n_classes = 9;

    Matrix<double> answers(std::vector<uint64_t>{n_objects, n_classes});
    Matrix<double> train(std::vector<uint64_t>{n_objects, n_features});

    RandomUniformGenerator idx_gen(0);
    auto indexes = idx_gen.random_permutation(n_objects);


    for (uint64_t idx = 0; idx < n_objects; ++idx) {

        for (uint64_t feature = 0; feature < n_features; ++feature) {
            train[{idx, feature}] = data[{idx, feature + 1}];
        }

        for (uint64_t class_idx = 0; class_idx < n_classes; ++class_idx) {
            answers[{idx, class_idx}] = uint64_t(data[{idx,0}]) == class_idx;
        }

    }


    train = shuffle_matrix(train, indexes);
    answers = shuffle_matrix(answers, indexes);

    auto loss = std::make_shared<CELossWithLogits>();

    FullyConnectedNN network(
            {
                    LayerOptions(n_features, 50, ACTIVATION::ReLU),
//                    LayerOptions(100, 60, ACTIVATION::ReLU),
                    LayerOptions(50, n_classes, ACTIVATION::Sigmoid)
            },
            static_cast<std::shared_ptr<Loss>>(loss),
            params
    );


    for (uint64_t epoch = 0; epoch < 1000; ++epoch) {

        auto predicted = network.forward(train);
        double loss_val = network.backward(predicted, answers);
        std::cout << loss_val << "\n";

        auto predicted_proba = softmax(predicted);
        auto predicted_labels = argmax(predicted, 0);
        auto labels = argmax(answers, 0);

        Accuracy acc;
        auto accuracy_val = acc.value(predicted_labels, labels);

        std::cout << epoch << " = epoch\n" << loss_val << " = loss\n" << accuracy_val << " = accuracy\n";
        std::cout << "\n";

    }
    //multi_classification_test();
    //binary_classification_test();

}
