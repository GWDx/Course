import numpy as np


# f(a) = y0
# a <= x <= b
def twoRungeKutta(f, a, b, y0, n, h):
    allX = np.linspace(a, b, n + 1)
    allY = np.zeros(n + 1)
    allY[0] = y0
    for i in range(n):
        k1 = f(allX[i], allY[i])
        k2 = f(allX[i] + h, allY[i] + h * k1)
        allY[i + 1] = allY[i] + h / 2 * (k1 + k2)
    return allY


def f(x, y):
    return y * np.sin(np.pi * x)


def main():
    # y'(x) = y sin pi x
    # y(0) = 1

    y0 = 1
    n = 10
    a = 0
    b = 2
    h = 0.2

    allY = twoRungeKutta(f, a, b, y0, n, h)
    print(allY)


main()
