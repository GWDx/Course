from math import ceil, floor
import numpy as np
import librosa
import soundfile
from numba import jit
import time


@jit
def sincIntepolation(rawData, d, size=10):
    sum = 0
    left = max(0, floor(d) - size)
    right = min(len(rawData), ceil(d) + size + 1)

    for i in range(left, right):
        sum += rawData[i] * np.sinc(d - i)
    return sum


@jit
def resample(rawData, sampleRate, newSampleRate):
    newLength = np.floor(len(rawData) / sampleRate * newSampleRate)
    allPosition = np.arange(newLength) / newSampleRate * sampleRate
    ans = np.array([sincIntepolation(rawData, i) for i in allPosition])
    return ans


def power(data):
    return np.mean(data**2)


def main():
    inputAudioFile = 'bach-musical-offering-crab-canon.wav'
    newSampleRate = 20000
    ansAudioFile = 'ans.wav'
    standardAudioFile = 'standard.wav'

    rawData, sampleRate = librosa.load(inputAudioFile, sr=None)

    print('resample begin')
    startTime = time.time()
    ansData = resample(rawData, sampleRate, newSampleRate)
    print('resample end')
    endTime = time.time()
    print('resample time:', endTime - startTime)
    print()

    soundfile.write(ansAudioFile, ansData, newSampleRate, subtype='PCM_24')
    print('power of ans:', power(ansData))

    standardData = librosa.resample(rawData, sampleRate, newSampleRate, res_type='sinc_best')
    soundfile.write(standardAudioFile, standardData, newSampleRate, subtype='PCM_24')
    print('power of standard:', power(standardData))


main()
