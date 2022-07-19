import numpy as np


# 改进 Eular 公式
def eular(f, g, a, b, y0, z0, n, h):
    allX = np.linspace(a, b, n + 1)
    allY = np.zeros(n + 1)
    allZ = np.zeros(n + 1)
    allBarY = np.zeros(n + 1)
    allBarZ = np.zeros(n + 1)
    allY[0] = y0
    allZ[0] = z0
    allBarY[0] = y0
    allBarZ[0] = z0
    for i in range(n):
        allBarY[i + 1] = allBarY[i] + h * f(allX[i], allY[i], allZ[i])
        allBarZ[i + 1] = allBarZ[i] + h * g(allX[i], allY[i], allZ[i])
        allY[i + 1] = allY[i] + h / 2 * (f(allX[i], allY[i], allZ[i]) + f(allX[i + 1], allBarY[i + 1], allBarZ[i + 1]))
        allZ[i + 1] = allZ[i] + h / 2 * (g(allX[i], allY[i], allZ[i]) + g(allX[i + 1], allBarY[i + 1], allBarZ[i + 1]))
    return allY, allZ


def f(x, y, z):
    return 0.09 * y * (1 - y / 20) - 0.45 * y * z


def g(x, y, z):
    return 0.06 * y * (1 - y / 15) - 0.001 * y * z


def main():
    # du/dt = 0.09u(1-u/20) - 0.45 uv
    # dv/dt = 0.06u(1-u/15) - 0.001 uv
    # u(0) = 1.6
    # v(0) = 1.2

    u0 = 1.6
    v0 = 1.2

    n = 10
    a = 0
    b = 2
    h = 0.2

    allY, allZ = eular(f, g, a, b, u0, v0, n, h)

    result = np.array([allY, allZ]).T
    print(result)


main()
