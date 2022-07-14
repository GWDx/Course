import numpy as np
import matplotlib.pyplot as plt
from numba import jit
from numpy import sqrt
import cv2


@jit
def getMaxGradDirection(grayData):
    rowCount, colCount = grayData.shape
    direction = np.zeros((rowCount - 2, colCount - 2), dtype=np.int32)
    maxGrad = np.zeros((rowCount - 2, colCount - 2))

    for i in range(1, rowCount - 1):
        for j in range(1, colCount - 1):
            allGrad = np.array([
                (grayData[i, j] - grayData[i - 1, j - 1]) / sqrt(2), grayData[i, j] - grayData[i - 1, j],
                (grayData[i, j] - grayData[i - 1, j + 1]) / sqrt(2), grayData[i, j] - grayData[i, j + 1],
                (grayData[i, j] - grayData[i + 1, j + 1]) / sqrt(2), grayData[i, j] - grayData[i + 1, j],
                (grayData[i, j] - grayData[i + 1, j - 1]) / sqrt(2), grayData[i, j] - grayData[i, j - 1]
            ])

            # allGrad = np.abs(allValue)
            direction[i - 1, j - 1] = np.argmax(allGrad)
            maxGrad[i - 1, j - 1] = np.max(allGrad)

    return direction, maxGrad


def plotGrad(direction, maxGrad, exportName=''):
    # normalize maxGrad
    maxGrad = maxGrad / np.max(maxGrad)
    plt.imshow(maxGrad, cmap='gray')
    plt.title('max gradient')

    # save 'maxGrad.png'
    plt.savefig(f'maxGrad{exportName}.png')

    # two subplots, 2 columns, 1 row
    # 1: show direction, use Hue
    # 2: show direction legend, colors of [0, 1, 2, 3, 4, 5, 6, 7] in plt.imshow
    plt.subplot(1, 2, 1)
    plt.imshow(direction, cmap='hsv', vmin=0, vmax=8)
    plt.title('direction')

    allDataRange = np.arange(0, 8)[:, np.newaxis]
    plt.subplot(1, 2, 2)
    plt.imshow(allDataRange, cmap='hsv', vmin=0, vmax=8)
    plt.xticks([])
    plt.yticks([0, 1, 2, 3, 4, 5, 6, 7],
               ['0 left up', '1 up', '2 right up', '3 right', '4 right down', '5 down', '6 left down', '7 left'])
    # y tick on the right
    plt.tick_params(axis='y', which='both', right=True, left=False, labelright=True, labelleft=False)
    plt.title('legend')
    plt.tight_layout()
    plt.savefig(f'direction{exportName}.png')


def getEigen(maxGrad, direction):
    gradRowCount, gradColCount = maxGrad.shape
    eigenGrad = np.zeros(gradColCount)
    eigenDirection = np.zeros(gradColCount, dtype=np.int32)
    eightPosition = np.zeros(gradColCount, dtype=np.int32)

    for i in range(gradRowCount):
        eigenGrad[i] = np.max(maxGrad[i])
        argMax = np.argmax(maxGrad[i])
        eightPosition[i] = argMax
        eigenDirection[i] = direction[i, argMax]
    return eigenGrad, eigenDirection, eightPosition


def highlight_cell(x, y, ax=None, **kwargs):
    rect = plt.Rectangle((x - .5, y - .5), 1, 1, fill=True, **kwargs)
    ax = ax or plt.gca()
    ax.add_patch(rect)
    return rect


def plotEigen(maxGrad, eigenGrad, eigenDirection, eightPosition, exportName=''):
    gradRowCount, gradColCount = maxGrad.shape
    # show maxGrad, highlight every eightPosition
    plt.figure(figsize=(10, 5), dpi=200)
    plt.imshow(maxGrad, cmap='gray')
    for i in range(gradColCount):
        highlight_cell(eightPosition[i], i, color='yellow', linewidth=0)
    plt.title(f'max gradient with eigen position{exportName}')
    plt.savefig(f'maxGradWithEigenPosition{exportName}.png')


def getGrayData(rawData):
    r = rawData[:, :, 0]
    g = rawData[:, :, 1]
    b = rawData[:, :, 2]
    grayData = 0.299 * r + 0.587 * g + 0.114 * b
    return grayData


def main():
    # resize 'raw.jpg' (128, 128))
    image = cv2.imread('raw.jpg')
    image = cv2.resize(image, (128, 128))
    # export 'resize.jpg'
    cv2.imwrite('resize.jpg', image)

    rawData = np.array(image)
    grayData = getGrayData(rawData)

    # show histogram
    plt.figure(figsize=(10, 5), dpi=200)
    plt.hist(grayData.flatten(), bins=256, range=(0, 255), fc='k', ec='k')
    plt.xlabel('gray value')
    plt.ylabel('number of pixels')
    plt.title('image histogram')
    # plt.show()
    # export 'histogram.png'
    plt.savefig('histogram.png')

    plt.clf()

    direction, maxGrad = getMaxGradDirection(grayData)
    plotGrad(direction, maxGrad)

    eigenGrad, eigenDirection, eightPosition = getEigen(maxGrad, direction)
    plotEigen(maxGrad, eigenGrad, eigenDirection, eightPosition)

    np.set_printoptions(threshold=10000)
    print(eigenGrad)
    print(eigenDirection)

    # gaussian blur 'raw.jpg'
    # kernel size : 50 * 50
    # use cv2
    image2 = cv2.imread('raw.jpg')
    image2 = cv2.GaussianBlur(image2, (9, 9), 0)
    image2 = cv2.resize(image2, (128, 128))
    cv2.imwrite('blur.jpg', image2)

    # import 'blur.jpg'
    rawData2 = np.array(image2)
    grayData2 = getGrayData(rawData2)

    direction2, maxGrad2 = getMaxGradDirection(grayData2)
    plotGrad(direction2, maxGrad2, ' GaussianBlur')

    eigenGrad2, eigenDirection2, eightPosition2 = getEigen(maxGrad2, direction2)
    plotEigen(maxGrad2, eigenGrad2, eigenDirection2, eightPosition2, ' GaussianBlur')

    #
    samePositionCount = np.sum(np.abs(eightPosition - eightPosition2) <= 2)
    print(samePositionCount, '/', eightPosition.size, '=', samePositionCount / eightPosition.size)


main()
