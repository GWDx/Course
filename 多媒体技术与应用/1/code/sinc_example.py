import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 99, 100)
y = np.cos(x / 3.4) + np.sin(x / 4.6)


def sincIntepolation(rawData, d, size):
    sum = 0
    left = max(0, np.floor(d).astype(int) - size)
    right = min(len(rawData), np.ceil(d).astype(int) + size + 1)

    for i in range(left, right):
        sum += rawData[i] * np.sinc(d - i)
    return sum


inteX = np.linspace(0, 99, 1000)
inteY1 = [sincIntepolation(y, d, 1) for d in inteX]
inteY4 = [sincIntepolation(y, d, 4) for d in inteX]

# show three plot in one figure, 3 columns, 1 row, with enough space
# 1 : scatter(x, y), mark 'input data'
# 2 : scatter(x, y), plot(inteX, inteY1), 'intepolation with size 1'
# 3 : scatter(x, y), plot(inteX, inteY4), 'intepolation with size 4'

# high resolution
plt.figure(figsize=(10, 8), dpi=200)

plt.subplot(3, 1, 1)
plt.scatter(x, y, s=8, c='red', marker='o')
# not show x axis
plt.xticks([])
plt.ylabel('y')
plt.title('input data')

plt.subplot(3, 1, 2)
plt.scatter(x, y, s=8, c='red', marker='o')
plt.plot(inteX, inteY1)
plt.xticks([])
plt.ylabel('y')
plt.title('intepolation with size 1')

plt.subplot(3, 1, 3)
plt.scatter(x, y, s=8, c='red', marker='o')
plt.plot(inteX, inteY4)
plt.xlabel('x')
plt.ylabel('y')
plt.title('intepolation with size 4')

# plt.show()
plt.savefig('example.png')
