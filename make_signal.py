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
parser.add_argument('-frequencies', metavar='f', type=int, nargs='+', default=10000,
                    help='frequencies that will be summed for the signal (default to 1000 Hz)')
parser.add_argument('-size', metavar='N', type=int, nargs=1, default=44100,
                    help='number of points (i.e. size of file), (default to 100000 points, must be greater than max frequency)')
parser.add_argument('-output', metavar='o', nargs=1, default='output.txt',
                    help='output file name (default to output.txt)')
parser.add_argument('-g', action='store_true',help='graph the output')

args = parser.parse_args()
args.frequencies = [args.frequencies] if type(args.frequencies) == int else args.frequencies

sample_size = 44100

time = np.linspace(0.0, 1.0, args.size)[:sample_size]
output = [0 for _ in time]

print('frequencies: {}'.format(args.frequencies))
print('sampling rate: {:.3f}'.format(args.size / 1))

for f in args.frequencies:
    for ii in range(len(time)):
        output[ii] += 0.5 * math.cos(2 * math.pi * f * time[ii])

with open(args.output, 'w') as f_out:
    for ii in range(len(time)):
        f_out.write('{},{}\n'.format(time[ii], output[ii]))

if args.g:
    plt.plot(time, output)
    plt.show()