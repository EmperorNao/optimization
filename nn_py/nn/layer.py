import numpy as np


class LinearLayer:

    def __init__(self, w, b, activation=None):
        self.w = w
        self.b = b
        self.activation = activation

        self.last_input = None
        self.last_output = None
        self.w_grad = None
        self.b_grad = None

    def __call__(self, x, eval=False):
        self.last_input = x
        out = x @ self.w + self.b

        if self.activation:
            self.last_output = out
            out = self.activation(out)

        return out

    def params_with_grad(self):
        return [
            (self.w, self.w_grad),
            (self.b, self.b_grad)
                ]
    
    def dict_params(self):
        return {"w": self.w, "b": self.b}

    def backward(self, grad):

        if self.activation:
            grad = np.multiply(grad, self.activation.backward(self.last_output))

        self.w_grad = self.last_input.transpose() @ grad
        self.b_grad = np.sum(grad, axis=0)

        return grad @ self.w.transpose()


class ResidualLayer:

    def __init__(self, w, b, activation=None):
        self.w = w
        self.b = b
        self.activation = activation

        self.last_input = None
        self.last_output = None
        self.w_grad = None
        self.b_grad = None

    def __call__(self, x, eval=False):
        self.last_input = x
        out = x @ self.w + self.b

        if self.activation:
            self.last_output = out
            out = self.activation(out)

        return x + out

    def params_with_grad(self):
        return [
            (self.w, self.w_grad),
            (self.b, self.b_grad)
                ]

    def dict_params(self):
        return {"w": self.w, "b": self.b}

    def backward(self, grad):

        if self.activation:
            grad = np.multiply(grad, self.activation.backward(self.last_output))

        self.w_grad = self.last_input.transpose() @ grad
        self.b_grad = np.sum(grad, axis=0)

        return grad + grad @ self.w.transpose()


class BNLayer:

    def __init__(self, mu=None, sigma=None, beta=None, gamma=None):
        self.mu = mu
        self.sigma = sigma
        self.lamda = 0.95

        self.gamma = gamma
        self.beta = beta
        self.gamma_grad = None
        self.beta_grad = None

        self.last_output = None
        self.last_ret_output = None

    def __call__(self, x, eval=False):

        mu = np.mean(x, 0)
        if self.mu is None:
            self.mu = mu

        if not eval:
            self.mu = self.mu * self.lamda + (1 - self.lamda) * mu

        sigma = np.var(x, 0)

        if self.sigma is None:
            self.sigma = sigma

        if not eval:
            self.sigma = self.sigma * self.lamda + (1 - self.lamda) * sigma

        out = (x - self.mu) / ((self.sigma + 1e-6) ** 0.5)

        if not eval:
            self.last_output = out

            if self.gamma is None:
                self.gamma = np.ones_like(out)
            if self.beta is None:
                self.beta = np.zeros_like(out)

            self.last_ret_output = out * self.gamma + self.beta

        return out

    def params_with_grad(self):
        return [
            (self.gamma, self.gamma_grad),
            (self.beta, self.beta_grad)
                ]

    def dict_params(self):
        return {"sigma": self.sigma, "mu": self.mu, "gamma": self.gamma, "beta": self.beta}

    def backward(self, grad):

        self.beta_grad = np.sum(self.last_output, 0)
        self.gamma_grad = np.sum(self.last_output * grad, 0)

        N = self.last_output.shape[0]
        dxhat = grad * self.gamma

        grad = (1. / N) * ((self.sigma + 1e-6) ** 0.5) * (N * dxhat - np.sum(dxhat, 0) -
                                                          self.last_output * np.sum(dxhat * self.last_output, 0))

        return grad









