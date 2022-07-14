import librosa
import numpy as np
import soundfile
from binarize import *
from play import *


def power(data):
    return np.mean(data**2)


def main():
    rawAudioFile = '荷塘月色.mp3'
    inputDataFile = 'inputData.txt'
    newSampleRate = 8000
    ansDataFile = 'ansData.txt'
    ansAudioFile = 'ans.wav'
    inverseAudioFile = 'inverse.wav'
    inputAudioFile = 'input.wav'

    rawData, sampleRate = librosa.load(rawAudioFile, sr=None)
    inputData = librosa.resample(rawData, sampleRate, newSampleRate)

    soundfile.write(inputAudioFile, inputData, newSampleRate)
    np.savetxt(inputDataFile, inputData)

    m = 255 / 15
    compand = compandor(inputData, m)
    ansData = classify(compand)

    np.savetxt(ansDataFile, ansData, '%d')
    normalizedAnsData = ansData / 8 - 1
    soundfile.write(ansAudioFile, normalizedAnsData, newSampleRate)

    inversedData = inverseTransform(ansData)
    soundfile.write(inverseAudioFile, inversedData, newSampleRate)


main()
