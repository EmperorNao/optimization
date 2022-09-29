import sys
from ast import literal_eval

import numpy as np

from nn.layer import LinearLayer, BNLayer, ResidualLayer
from nn.functional import create_activation


class FullyConnectedNN:

    def __init__(self,
                 layers_options,
                 loss=None,
                 optimizer=None):

        self.layers_options = layers_options
        self.loss = loss
        self.optimizer = optimizer

        self.layers = []
        for option in self.layers_options:
            if option['layer_type'] == 'linear':
                input = option['input']
                output = option['output']
                activation = option.get('activation')

                w = np.random.normal(loc=0, scale=0.05, size=(input, output))
                b = np.random.normal(loc=0, scale=0.05, size=(1, output))
                self.layers.append(
                    LinearLayer(w, b, create_activation(activation))
                )
            elif option['layer_type'] == 'residual':
                input = option['input']
                output = option['output']
                activation = option.get('activation')

                w = np.random.normal(loc=0, scale=0.05, size=(input, output))
                b = np.random.normal(loc=0, scale=0.05, size=(1, output))
                self.layers.append(
                    ResidualLayer(w, b, create_activation(activation))
                )
            elif option['layer_type'] == 'bn':
                self.layers.append(
                    BNLayer()
                )

    def __call__(self, x, eval=False):

        out = x
        for layer in self.layers:
            out = layer(out, eval)

        return out

    def backward(self, predicted, y):

        loss_value = self.loss(predicted, y)
        grad = self.loss.backward(predicted, y)

        for layer in reversed(self.layers):
            grad = layer.backward(grad)

        self.optimizer(self.layers)

        return loss_value

    def save(self, filename):
        repr = {'layers_options': self.layers_options}
        np.set_printoptions(threshold=sys.maxsize)

        params = []
        for layer in self.layers:
            layer_params = {}
            for paramname, param in layer.dict_params().items():
                r = str(None)
                if param is not None:
                    r = np.array2string(param, separator=', ')
                layer_params[paramname] = r
            params.append(layer_params)

        repr['parameters'] = params

        with open(filename, "w") as file:
            file.write(str(repr))

    def load(self, filename):
        data = {}
        with open(filename) as file:
            data = literal_eval(file.read())

        self.layers_options = data['layers_options']
        self.layers = []

        for idx, params in enumerate(data['parameters']):

            option = self.layers_options[idx]

            if option['layer_type'] == 'linear':
                activation = option.get('activation')
                w = np.array(literal_eval(params['w']), ndmin=2)
                b = np.array(literal_eval(params['b']), ndmin=2)
                self.layers.append(
                    LinearLayer(w, b, create_activation(activation))
                )

            elif option['layer_type'] == 'residual':
                activation = option.get('activation')
                w = np.array(literal_eval(params['w']), ndmin=2)
                b = np.array(literal_eval(params['b']), ndmin=2)
                self.layers.append(
                    ResidualLayer(w, b, create_activation(activation))
                )
            elif option['layer_type'] == 'bn':
                beta = params['beta']
                if beta is not None:
                    beta = np.array(literal_eval(beta), ndmin=1)

                gamma = params['gamma']
                if gamma is not None:
                    gamma = np.array(literal_eval(gamma), ndmin=1)

                mu = params['mu']
                if mu is not None:
                    mu = np.array(literal_eval(mu), ndmin=1)

                sigma = params['sigma']
                if sigma is not None:
                    sigma = np.array(literal_eval(sigma), ndmin=1)

                self.layers.append(
                    BNLayer(mu=mu, sigma=sigma, beta=beta, gamma=gamma)
                )





