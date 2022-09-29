import numpy as np


def generate_batches(X, y, batch_size):

    assert len(X) == len(y)
    np.random.seed(42)
    X = np.array(X)
    y = np.array(y)
    perm = np.random.permutation(len(X))

    for batch_start in range(0, X.shape[0] // batch_size):
        yield X[perm][batch_start * batch_size: (batch_start + 1) * batch_size], y[perm][batch_start * batch_size: (batch_start + 1) * batch_size]