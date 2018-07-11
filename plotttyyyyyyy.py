import matplotlib.pyplot as plt

xs = []
ts1 = []
ts2 = []
with open("output_1kHz.txt", "r") as fx:
    for line in fx:
        xs.append(float(line.strip()))

with open("updated_goertzel_1k.txt", "r") as tx1:
    for line in tx1:
        ts1.append(float(line.strip()))

with open("updated_goertzel_10k.txt", "r") as tx2:
    for line in tx2:
        ts2.append(float(line.strip()))


indicies = [i for i in range(len(xs))]

fig, ax1 = plt.subplots()
ax1.plot(indicies, xs)
ax2 = ax1.twinx()
ax2.plot(indicies, ts1, color='orange')
ax2.plot(indicies, ts2, color='red')

# resetsample = 2
# n=0
# while (n * resetsample < 1024):
#     n += 1
#     plt.axvline(x=n*resetsample, color='black', linestyle=':', linewidth=1)

plt.show()