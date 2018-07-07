'''
Python prototype of digital filtering
'''

import math
from Typing import List

Fs = 1000000 # sampling rate
N = 1024 # number of samples of fourier analysis

'''
X[k] = Re(X[k]) + Im(X[k]) = Sum(n=0 to N-1)(x[n]cos(k * n * 2 * pi / N)) - Sum(n=0 to N-1)(x[n]sin(k * n * 2 * pi / N))
'''
def single_freq_fourier(x: List, f: int) -> float:
    k = round(f / (Fs * N))
    ReX, ImX = 0, 0
    for n in range(N - 1):
        ReX += x[n] * math.cos(k * n * 2 * math.pi / N)
        ImX += x[n] * math.sin(k * n * 2 * math.pi / N)
    return ReX + ImX

