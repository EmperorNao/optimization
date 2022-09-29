import numpy as np


class SGD:

    def __init__(self, lr):
        self.lr = lr

    def __call__(self, layers):

        for layer in layers:

            for param, grad in layer.params_with_grad():
                param -= self.lr * grad

