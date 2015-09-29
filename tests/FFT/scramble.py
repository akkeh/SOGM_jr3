import numpy as np


def scramble(x):
    N = len(x)
    even = x[::2]
    oneven = x[1::2]
    if N > 2:
        even = scramble(even)
        oneven = scramble(oneven)
    return np.append(even, oneven)
