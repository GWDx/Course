import numpy as np
import matplotlib.pyplot as plt


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

    # 高度>0 表示 从左到右、从下到上
    if biHeight > 0:
        data = np.flip(data, 0)
    return data


def main():
    with open('WomanWithAParasol.bmp', 'rb') as f:
        raw = f.read()

    # file header
    bfType = raw[0:2]  # 2
    bfSize = raw[2:6]  # 4
    bfReserved1 = raw[6:8]  # 2
    bfReserved2 = raw[8:10]  # 2
    bfOffBits = raw[10:14]  # 4

    if bfType != b'BM':
        exit('Not a BMP file')

    bfOffBits = int.from_bytes(bfOffBits, 'little')

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
    if biCompression != 0:
        exit('Only support uncompressed image')

    rawData = raw[bfOffBits:]
    data = convertBMPDataToValue(rawData, biBitCount, biHeight, biWidth)

    plt.figure(figsize=(10, 10), dpi=200)
    plt.imshow(data)
    plt.savefig('result.png')

    redData = data[:, :, 0]
    greenData = data[:, :, 1]
    blueData = data[:, :, 2]

    plt.figure(figsize=(10, 4), dpi=200)
    # show red green blue
    plt.subplot(1, 3, 1)
    plt.imshow(redData, cmap='gray')
    plt.title('red')

    plt.subplot(1, 3, 2)
    plt.imshow(greenData, cmap='gray')
    plt.yticks([])
    plt.title('green')

    plt.subplot(1, 3, 3)
    plt.imshow(blueData, cmap='gray')
    plt.yticks([])
    plt.title('blue')
    # save 'channels.png'
    plt.savefig('channels.png')
    # plt.show()

    # print info biWidth, biHeight, biBitCount
    print('biWidth:', biWidth)
    print('biHeight:', biHeight)
    print('biBitCount:', biBitCount)


main()
