import librosa
import numpy as np
import matplotlib.pyplot as plt
import soundfile


def damingDataSince(data, since):
    # ans 0, size composeLength
    ans = np.zeros((composeLength + 1))
    for i in range(since + len(data), composeLength):
        ans[i] = 0.999**(i - since - len(data)) * data[(i - since) % len(data)]
    for i in range(since, since + len(data)):
        ans[i] = data[(i - since) % len(data)]
    return ans


# load audio/zh.wav, audio/u.wav, audio/ang.wav and audio/zhuang.wav
zhAudio, sr1 = librosa.load('audio/zh.wav')
uAudio, sr2 = librosa.load('audio/u.wav')
angAudio, sr3 = librosa.load('audio/ang.wav')
zhuangAudio, sr4 = librosa.load('audio/zhuang.wav')

# change audio length, use librosa, keep frequency
# composeLength = int(sr1 * 2)
# zhAudio = librosa.resample(zhAudio, sr1, composeLength)

normalizedZhData = zhAudio
normalizedUData = uAudio[4000:4800]
normalizedAngData = angAudio[1000:3500]

# .9^i * normalizedZhData[i]
# dampingZhData = librosa.util.normalize(damingData(normalizedZhData))
# dampingUData = librosa.util.normalize(damingData(normalizedUData))
# dampingAngData = librosa.util.normalize(damingData(normalizedAngData))

composeLength = len(normalizedZhData) + len(normalizedUData) + len(normalizedAngData)

dampingZhData = librosa.util.normalize(damingDataSince(normalizedZhData, 0))
dampingUData = librosa.util.normalize(damingDataSince(normalizedUData, len(normalizedZhData)))
dampingAngData = librosa.util.normalize(damingDataSince(normalizedAngData,
                                                        len(normalizedZhData) + len(normalizedUData)))

composeData = dampingZhData + dampingUData * .3 + dampingAngData * .5
normalizedComposeData = librosa.util.normalize(composeData)

print(len(normalizedComposeData))
print(len(zhuangAudio))

plt.plot(normalizedComposeData)
plt.plot(librosa.util.normalize(zhuangAudio))
plt.show()

# export composeData to audio/compose.wav
soundfile.write('audio/compose.wav', normalizedComposeData, sr1)

plt.plot(dampingZhData)
plt.plot(normalizedZhData)
plt.show()
plt.plot(dampingUData)
plt.plot(normalizedUData)
plt.show()
plt.plot(dampingAngData)
plt.plot(normalizedAngData)
plt.show()
