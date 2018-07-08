'''
Python prototype of digital filtering
'''

import math
import matplotlib.pyplot as plt

Fs = 100000 # sampling rate
N = 100000 # number of samples of fourier analysis

'''
X[k] = Re(X[k]) + Im(X[k]) = Sum(n=0 to N-1)(x[n]cos(k * n * 2 * pi / N)) - Sum(n=0 to N-1)(x[n]sin(k * n * 2 * pi / N))
'''
def single_freq_fourier(x, n, f: int) -> float:
    # k = f * N / Fs
    k = f
    ReX = x * math.cos(k * n * 2 * math.pi / Fs) 
    ImX = x * math.sin(k * n * 2 * math.pi / Fs)
    return ReX - ImX

def get_fourier(file_name='output.txt'):
    X = []
    Ks= []
    with open(file_name) as f:
        for K in range(0, N):
            n = 0
            X_new = 0
            for line in f:
                line = line.split(',')
                x = float(line[1].strip(r'\s'))
                X_new += single_freq_fourier(x, n, 1000)
                n += 1
            Ks.append(K)
            X.append(X_new)
    plt.plot(Ks, X)
    print(X)
    plt.show()


if __name__ == '__main__':
    get_fourier()