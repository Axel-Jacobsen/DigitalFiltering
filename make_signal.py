'''
Used to create waves to test fourier analysis files

structure of output file:
    - Rows of 'time, value' pairs
'''

import argparse
import math
import numpy as np
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Create sine/cosine data files')
parser.add_argument('-frequencies', metavar='f', type=int, nargs='+', default=1000,
                    help='frequencies that will be summed for the signal (default to 1000 Hz)')
parser.add_argument('-g', action='store_true',help='graph the output')

args = parser.parse_args()
args.frequencies = [args.frequencies] if type(args.frequencies) == int else args.frequencies

sample_size = 1024 * 10
SAMPLE_FREQ = 100000
FILE_OUT = 'output_1kHz.txt'

time = np.linspace(0.0, 1, SAMPLE_FREQ)[:sample_size]
output = [0 for _ in time]

print('frequencies: {}'.format(args.frequencies))
print('sampling rate: {:.3f}'.format(SAMPLE_FREQ / 1))
print('file out: {}'.format(FILE_OUT))

for f in args.frequencies:
    for ii in range(math.floor(len(time) / 2)):
        output[ii] += 0.5 * math.cos(2 * math.pi * 1000 * time[ii])
    for ii in range(math.floor(len(time) / 2), math.floor(len(time) / 2)):
        output[ii] += 0.5 * math.cos(2 * math.pi * 500 * time[ii])
    for ii in range(math.floor(len(time) / 2), math.floor(3 * len(time) / 4)):
        output[ii] += 0.5 * math.cos(2 * math.pi * 1000 * time[ii])
    for ii in range(math.floor(3 * len(time) / 4), len(time)):
        output[ii] += 0.5 * math.cos(2 * math.pi * 10000 * time[ii])


with open(FILE_OUT, 'w') as f_out:
    for ii in range(len(time)):
        f_out.write('{}\n'.format(output[ii]))

if args.g:
    plt.plot(time, output)
    plt.show()