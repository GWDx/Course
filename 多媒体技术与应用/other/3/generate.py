import numpy as np
import soundfile

sampleRate = 44100


def generate(frequency):
    second = 30

    phi = np.array([4, 2, 1])
    n = len(phi)
    np.random.seed(0)
    omega = np.random.rand(n) * 2 * np.pi
    # phi = np.random.rand(n)

    length = sampleRate * second + 1
    allTime = np.linspace(0, second, length)

    data = np.zeros(length)
    for i in range(n):
        data += phi[i] * np.sin((i + 1) * allTime * frequency + omega[i])

    return data


def main():
    frequency = 400 * 2 * np.pi
    data1 = generate(frequency)
    normalizedData1 = data1 / data1.max() * .8
    soundfile.write('1.wav', normalizedData1, sampleRate)

    frequencyRatio = 1.6
    data2 = generate(frequency * frequencyRatio)
    normalizedData2 = data2 / data2.max() * .8
    soundfile.write('2.wav', normalizedData2, sampleRate)

    amplitudeRatio = .4
    data3 = normalizedData1 * amplitudeRatio
    soundfile.write('3.wav', data3, sampleRate)

    pass


main()
