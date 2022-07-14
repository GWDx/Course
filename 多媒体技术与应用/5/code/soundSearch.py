import librosa
import numpy as np
from numba import jit


# 量化
def dateToMark(data):
    # data[i] - data[i-1]
    diff = np.diff(data)
    angle = np.arctan(diff * 20)

    # pi/2 to 63, -pi/2 to 0
    angleMark = ((angle + np.pi / 2) / np.pi * 64).astype(int)
    return angleMark


# DTW
@jit
def dtw(x, y):
    m = len(x)
    n = len(y)

    d = np.zeros((m + 1, n + 1))
    for i in range(m + 1):
        for j in range(n + 1):
            d[i, j] = abs(x[i - 1] - y[j - 1])

    # DTW = inf, size = (m+1, n+1)
    DTW = np.inf * np.ones((m + 1, n + 1))
    DTW[0, 0] = 0
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            cost = d[i, j]
            DTW[i, j] = cost + min(DTW[i - 1, j], DTW[i, j - 1], DTW[i - 1, j - 1])
    # print(g)
    return DTW[m, n]


def main():
    sampleRate = 6000
    allTemplateMark = []
    for i in range(10):
        rawData = librosa.load(f'template/{i}.wav', sr=sampleRate)
        templateData = rawData[0]
        templateMark = dateToMark(templateData)
        allTemplateMark.append(templateMark)
        # delta = 1000
        # data = rawData[::delta]

    for testFile in ['test1.wav', 'test4.wav']:
        testRawData = librosa.load(testFile, sr=sampleRate)
        testData = testRawData[0]
        teatMark = dateToMark(testData)

        print('testFile:', testFile)
        allDtwResult = []
        for i in range(10):
            dtwResult = dtw(allTemplateMark[i], teatMark)
            print(f'{i} : {dtwResult}')
            allDtwResult.append(dtwResult)

        # max position
        maxPos = np.argmin(allDtwResult)
        print()
        print(f'result:  {maxPos} : {allDtwResult[maxPos]}')


def test():
    x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    y = [1, 2, 3, 4, 5, 6, 10, 9, 8, 7]
    print(dtw(x, y))


main()
