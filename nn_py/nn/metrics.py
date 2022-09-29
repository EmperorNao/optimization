import numpy as np


def accuracy(predicted, labels):
    return np.mean(predicted == labels)