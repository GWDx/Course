import cv2
import numpy as np
import matplotlib.pyplot as plt

video = cv2.VideoCapture('2021青海湖茶卡盐湖.mp4')

all_frames = []

while True:
    ret, frame = video.read()
    if ret:
        all_frames.append(frame)
    else:
        break

allImageHistogram = []

for i in range(len(all_frames)):
    # cv2.imwrite('frames/frame%d.jpg' % i, all_frames[i])

    # image gray histogram
    frame = all_frames[i]
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    imageHistogram = cv2.calcHist([frame], [0], None, [8], [0, 256])
    allImageHistogram.append(imageHistogram)

allImageHistogram = np.array(allImageHistogram)
allImageHistogram = allImageHistogram[:, :, 0]

# diff allImageHistogram
diff = np.diff(allImageHistogram, axis=0)

allFrameNorm = np.linalg.norm(diff, axis=1)

plt.plot(allFrameNorm)
plt.xlabel('frame')
plt.ylabel('difference')
plt.title('视频连续两帧差分的绝对值')
plt.savefig('allFrameNorm.png')

pass
