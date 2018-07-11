import matplotlib.pyplot as plt

'''
This file is written to plot frequency output, the output from the goertzel stream, and the 
derivative of the output from the goertzel
'''

xs  = []
ts1 = []
d   = []

with open("variable_output.txt", "r") as fx:
    for line in fx:
        xs.append(float(line.strip()))

with open("circular_goertzel_stream.txt", "r") as tx1:
    for line in tx1:
        ts1.append(float(line.strip()))

with open("deriv_goertzel_strm.txt", "r") as d1:
    for line in d1:
        d.append(float(line.strip()))

indicies = [i for i in range(len(xs))]

fig, ax1 = plt.subplots()
ax2 = ax1.twinx()
ax1.plot(indicies, xs, linewidth=2)
ax2.plot(indicies, ts1, color='orange', linewidth=2)

# plot derivative every 100 values (update rate for derivative), plus 1023 (time to initially fill buffer)
ax2.plot([i*100 + 1023 for i in range(len(d))], d, color='red', linewidth=2)


plt.axvline(x=1023, color='black', linestyle='-', linewidth=2)

plt.show()