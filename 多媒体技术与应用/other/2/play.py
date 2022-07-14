import soundfile
import numpy as np
from sympy import *


def inverseTransform(ansData):
    m = 255 / 15
    allValue = np.linspace(-15 / 16, 15 / 16, 16)
    allX = np.array([])

    for value in allValue:
        x = symbols('x')
        ansX = nsolve(sign(x) * log(1 + m * abs(x)) / log(1 + m) - value, x, 0)
        allX = np.append(allX, ansX)

    audioData = allX.astype(np.float64)[ansData]
    return audioData


# main
if __name__ == '__main__':
    ansData = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
    print(inverseTransform(ansData))
