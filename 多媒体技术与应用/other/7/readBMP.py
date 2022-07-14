import numpy as np
from numpy import cos, pi
import matplotlib.pyplot as plt
from numba import jit
from sklearn.preprocessing import normalize


def convertBMPDataToValue(rawData, biBitCount, biHeight, biWidth):
    colorBit = biBitCount // 8
    height = abs(biHeight)
    width = biWidth
    data = np.zeros((height, width, colorBit), dtype=np.uint8)
    dataIndex = 0
    if colorBit == 3:
        for i in range(height):
            for j in range(width):
                data[i, j, 2] = int.from_bytes(rawData[dataIndex:dataIndex + 1], 'little')  # blue
                data[i, j, 1] = int.from_bytes(rawData[dataIndex + 1:dataIndex + 2], 'little')  # green
                data[i, j, 0] = int.from_bytes(rawData[dataIndex + 2:dataIndex + 3], 'little')  # red
                dataIndex += 3
            # align to 4 bytes
            dataIndex += (4 - (width * 3) % 4) % 4
    else:
        for i in range(height):
            for j in range(width):
                data[i, j, 2] = int.from_bytes(rawData[dataIndex:dataIndex + 1], 'little')  # blue
                data[i, j, 1] = int.from_bytes(rawData[dataIndex + 1:dataIndex + 2], 'little')  # green
                data[i, j, 0] = int.from_bytes(rawData[dataIndex + 2:dataIndex + 3], 'little')  # red
                data[i, j, 3] = int.from_bytes(rawData[dataIndex + 3:dataIndex + 4], 'little')  # alpha
                dataIndex += 4
    return data


@jit
def rgbToYcbcr(r, g, b):
    y = 0.299 * r + 0.587 * g + 0.114 * b
    cb = -0.1687 * r - 0.3313 * g + 0.5 * b + 128
    cr = 0.5 * r - 0.4187 * g - 0.0813 * b + 128
    return y, cb, cr


def fdctFunction(u, v, block):
    if u == v == 0:
        cu = cv = 1 / np.sqrt(2)
    else:
        cu = cv = 1
    total = sum(block[i, j] * cos((2 * i + 1) * u * pi / 16) * cos((2 * j + 1) * v * pi / 16)  #
                for i in range(8) for j in range(8))
    result = cu * cv / 4 * total
    return result


def blockDCT(block):  # size = 8 * 8
    dctData = np.zeros((8, 8), dtype=np.float64)
    for u in range(8):
        for v in range(8):
            dctData[u, v] = fdctFunction(u, v, block)
    return dctData


def dctTransform(data):  # single channel
    # split 8 * 8 block
    blockXNum = data.shape[0] // 8
    blockYNum = data.shape[1] // 8
    dctData = np.zeros((blockXNum, blockYNum, 8, 8), dtype=np.float64)
    for i in range(blockXNum):
        for j in range(blockYNum):
            block = data[i * 8:(i + 1) * 8, j * 8:(j + 1) * 8]
            dctData[i, j] = blockDCT(block)
    return dctData


def zigZagScanBlock(data):
    result = np.zeros(64, dtype=np.float64)
    # matrix = np.zeros((8, 8), dtype=np.float64)
    zigZagIndex = 0
    x = 0  # 行
    y = 0  # 列
    direction = True  # True: 向右上，False: 向左下
    while zigZagIndex < 64:
        if y == 7:
            direction = False
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            x += 1
        elif x == 7:
            direction = True
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            y += 1
        elif x == 0:
            direction = False
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            y += 1
        elif y == 0:
            direction = True
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            x += 1

        if direction:
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            x -= 1
            y += 1
        else:
            # matrix[x, y] = zigZagIndex
            result[zigZagIndex] = data[x, y]
            zigZagIndex += 1
            x += 1
            y -= 1

    # print(matrix)
    return result


def main():
    with open('bmp/1.bmp', 'rb') as f:
        raw = f.read()

    # file header
    bfType = raw[0:2]  # 2
    bfSize = raw[2:6]  # 4
    bfReserved1 = raw[6:8]  # 2
    bfReserved2 = raw[8:10]  # 2
    bfOffBits = raw[10:14]  # 4

    bfOffBits = int.from_bytes(bfOffBits, 'little')

    if bfType != b'BM':
        exit('Not a BMP file')

    # info header
    biSize = raw[14:18]  # 4
    biWidth = raw[18:22]  # 4
    biHeight = raw[22:26]  # 4
    biPlanes = raw[26:28]  # 2
    biBitCount = raw[28:30]  # 2
    biCompression = raw[30:34]  # 4
    biSizeImage = raw[34:38]  # 4
    biXPelsPerMeter = raw[38:42]  # 4
    biYPelsPerMeter = raw[42:46]  # 4
    biClrUsed = raw[46:50]  # 4
    biClrImportant = raw[50:54]  # 4

    biWidth = int.from_bytes(biWidth, 'little')
    biHeight = int.from_bytes(biHeight, 'little')
    biBitCount = int.from_bytes(biBitCount, 'little')
    biCompression = int.from_bytes(biCompression, 'little')

    if biBitCount not in [24, 32]:
        exit('Only support 24 and 32 bit image')
    # if biCompression != 0:
    #     exit('Only support uncompressed image')

    rawData = raw[bfOffBits:]
    data = convertBMPDataToValue(rawData, biBitCount, biHeight, biWidth)

    # 高度>0 表示 从左到右、从下到上
    if biHeight > 0:
        data = np.flip(data, 0)

    redData = data[:, :, 0]
    greenData = data[:, :, 1]
    blueData = data[:, :, 2]

    y, cb, cr = rgbToYcbcr(redData, greenData, blueData)

    # dctY = dctTransform(y)
    # zigZagY = zigZagScan(quantizedY)

    colorQuantizeTable = np.array([
        [17, 18, 24, 47, 99, 99, 99, 99],
        [18, 21, 26, 66, 99, 99, 99, 99],
        [24, 26, 56, 99, 99, 99, 99, 99],
        [47, 66, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
        [99, 99, 99, 99, 99, 99, 99, 99],
    ])

    lightQuantizeTable = np.array([
        [16, 11, 10, 16, 24, 40, 51, 61],
        [12, 12, 14, 19, 26, 58, 60, 55],
        [14, 13, 16, 24, 40, 57, 69, 56],
        [14, 17, 22, 29, 51, 87, 80, 62],
        [18, 22, 37, 56, 68, 109, 103, 77],
        [24, 35, 55, 64, 81, 104, 113, 92],
        [49, 64, 78, 87, 103, 121, 120, 101],
        [72, 92, 95, 98, 112, 100, 103, 99],
    ])

    yMinIndexX, yMinIndexY = np.unravel_index(y.argmin(), y.shape)
    yMinBlock = y[yMinIndexX:yMinIndexX + 8, yMinIndexY:yMinIndexY + 8]
    yMinBlockDCT = blockDCT(yMinBlock)

    normalizedYMinBlockDCT = normalize(yMinBlockDCT)
    print(normalizedYMinBlockDCT, '\n')
    yMinBlockDCTQuantized = np.round(normalizedYMinBlockDCT * lightQuantizeTable)
    print(yMinBlockDCTQuantized, '\n')
    yMinBlockZigZag = zigZagScanBlock(yMinBlockDCTQuantized)
    print(yMinBlockZigZag)

    # # show red green blue
    # plt.subplot(1, 3, 1)
    # plt.imshow(redData, cmap='gray')
    # plt.subplot(1, 3, 2)
    # plt.imshow(greenData, cmap='gray')
    # plt.subplot(1, 3, 3)
    # plt.imshow(blueData, cmap='gray')
    # plt.show()


main()
