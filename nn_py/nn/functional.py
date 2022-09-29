import numpy as np
from math import log, exp


def sigmoid(x):
    return 1 / (1 + exp(-x))


def map_sigmoid(array):
    return np.array([sigmoid(el) for el in array])


def softmax(matrix):
    expo = np.exp(matrix)
    sum = np.expand_dims(np.sum(expo, axis=1), -1)
    return expo / (sum + 1e-6)


class ReLU:

    def __call__(self, x):
        return np.maximum(0, x)

    def backward(self, x):
        return (x > 0).astype(float)


class BCELoss:

    def __call__(self, x, y):
        proba = map_sigmoid(x)
        out = np.multiply(y, np.log(proba + 1e-6)) + np.multiply((1 - y), np.log(1 - proba + 1e-6))
        return -1 * np.mean(out)

    def backward(self, x, y):
        proba = map_sigmoid(x)
        return np.expand_dims((proba - y), -1)


class CELoss:

    def __call__(self, x, y):
        proba = softmax(x)
        out = np.multiply(y, np.log(proba + 1e-6))
        return -1 * np.mean(out)

    def backward(self, x, y):
        proba = softmax(x)
        return proba - y


def create_activation(activation):
    if not activation:
        return None

    if activation.lower() == "relu":
        return ReLU()
    else:
        raise ValueError("Wrong activation: %s" % activation)
