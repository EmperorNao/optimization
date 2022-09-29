import numpy as np
import pandas as pd


from nn.nn import FullyConnectedNN
from nn.functional import BCELoss, CELoss, map_sigmoid
from nn.optim import SGD
from nn.metrics import accuracy


from utils.utils import generate_batches



train_data = pd.read_csv("/home/emperornao/projects/optimization/datasets/mnist_train.csv")

train_cols = train_data.columns[1:]
label_col = train_data.columns[0]

labels = train_data[label_col].to_numpy()
train = train_data[train_cols].to_numpy()

ohe_labels = np.zeros((labels.shape[0], 10))
for idx, el in enumerate(labels):
    ohe_labels[idx, el] = 1


loss = CELoss()
optim = SGD(1e-3)

nn = FullyConnectedNN(
    [
        {'layer_type': 'bn'},
        {'layer_type': 'linear', 'input': 784, 'output': 1000},
        {'layer_type': 'bn'},
        {'layer_type': 'residual', 'input': 1000, 'output': 1000, 'activation': 'relu'},
        {'layer_type': 'bn'},
        {'layer_type': 'linear', 'input': 1000, 'output': 250, 'activation': 'relu'},
        {'layer_type': 'bn'},
        {'layer_type': 'linear', 'input': 250, 'output': 10},
     ],
    loss,
    optim
)


for epoch in range(5):
    try:
        for n_iter, batch in enumerate(generate_batches(train, ohe_labels, 256)):
            x_batch, y_batch = batch
            proba = nn(x_batch)
            loss_val = nn.backward(proba, y_batch)

            predictions = proba
            acc = accuracy(predictions.argmax(axis=1), y_batch.argmax(axis=1))

            print("iter %i, loss = %f, acc = %f" % (n_iter, loss_val, acc))
    except:
        nn.save("/home/emperornao/projects/optimization/nn_py/model" + str(epoch))





