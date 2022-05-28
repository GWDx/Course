import numpy as np
import mnist
import matplotlib.pyplot as plt
import sys
import time
from mpi4py import MPI

comm = MPI.COMM_WORLD

size = comm.Get_size()
rank = comm.Get_rank()

rawTrainX, rawTrainTarget, rawTestX, rawTestTarget = mnist.load()

trainX = np.array(rawTrainX)
trainTarget = np.array(rawTrainTarget, dtype=np.int32)
testX = np.array(rawTestX)
testTarget = np.array(rawTestTarget, dtype=np.int32)


def relu(x):
    return np.maximum(x, 0)


def reluDerivative(x, y):
    return int(y > 0)


# log_softmax(x_i) = (x_i - M) - log(sum(exp(x_j - M)))
# M = max(x_j)
def logSoftmax(x):
    M = np.max(x)
    ans = (x - M) - np.log(np.sum(np.exp(x - M)) + 1e-10)
    return ans


def nllLoss(y, target):
    targetOneHot = np.eye(10)[target]
    ansMatrix = y * targetOneHot
    ans = -np.sum(ansMatrix)
    return ans


# numpy random seed
np.random.seed(0)

A1 = np.zeros((784, 200))
b1 = np.abs(np.random.randn((200)))
A2 = np.zeros((200, 50))
b2 = np.abs(np.random.randn((50)))
A3 = np.zeros((50, 10))
b3 = np.abs(np.random.randn((10)))


def forward(x: np.array):  # shape (784)
    global Ab1, r1, Ab2, r2, Ab3, y
    Ab1 = x @ A1 + b1  # shape (200)
    r1 = relu(Ab1)  # shape (200)
    Ab2 = r1 @ A2 + b2  # shape (50)
    r2 = relu(Ab2)  # shape (50)
    Ab3 = r2 @ A3 + b3  # shape (10)
    y = logSoftmax(Ab3)  # shape (10)
    return y


def testLossDecrease(x, target, loss, label):
    newLoss = nllLoss(forward(x), target)
    if newLoss > loss:
        sys.stderr.write(' ' + label)


m_A3 = np.zeros((50, 10))
v_A3 = np.zeros((50, 10))
m_b3 = np.zeros(10)
v_b3 = np.zeros(10)
m_A2 = np.zeros((200, 50))
v_A2 = np.zeros((200, 50))
m_b2 = np.zeros(50)
v_b2 = np.zeros(50)
m_A1 = np.zeros((784, 200))
v_A1 = np.zeros((784, 200))
m_b1 = np.zeros(200)
v_b1 = np.zeros(200)

eps = 1e-8
beta1 = 0.9
beta2 = 0.999
batchSize = 20


def backward(x, target):
    global m_A3, m_b3, m_A2, m_b2, m_A1, m_b1, v_A3, v_b3, v_A2, v_b2, v_A1, v_b1, updateCount
    global A3, b3, A2, b2, A1, b1, delta_A3, delta_b3, delta_A2, delta_b2, delta_A1, delta_b1
    # d loss / d y[i] = -targetOneHot[i]
    targetOneHot = np.eye(10)[target]
    loss_y = -targetOneHot  # shape (10)

    # d y[i] / d Ab3[j] = if i == j then 1 - E^y[i] else -E^y[j]
    y_Ab3 = np.eye(10) - np.exp(y)  # shape (10, 10)

    # d Ab3[j] / d r2[i] = A3[i, j]
    # d Ab3[j] / d A3[i, j] = r2[i]
    # d Ab3[j] / d b3[j] = 1
    Ab3_r2 = A3.T  # shape (10, 50)
    # Ab3_A3[i,j,k] = if i==k then r2[j] else 0
    Ab3_A3 = r2  # shape (50)
    Ab3_b3 = 1

    # d r2[i] / d Ab2[i] = int(Ab2[i] > 0)
    r2_Ab2 = r2 >= 0

    # d Ab2[i] / d r1[i] = A2[i, j]
    # d Ab2[i] / d A2[i, j] = r1[i]
    # d Ab2[i] / d b2[j] = 1
    Ab2_r1 = A2.T  # shape (50, 200)
    Ab2_A2 = r1  # shape (200)
    Ab2_b2 = 1  # shape (50)

    # d r1[i] / d Ab1[i] = int(Ab1[i] > 0)
    r1_Ab1 = r1 >= 0

    # d Ab1[i] / d x[i] = A1[i, j]
    # d Ab1[i] / d A1[i, j] = x[i]
    # d Ab1[i] / d b1[j] = 1
    Ab1_x = A1.T  # shape (200, 784)
    Ab1_A1 = x  # shape (784)
    Ab1_b1 = 1  # shape (200)

    # evaluate grad
    loss_Ab3 = loss_y @ y_Ab3  # shape (10)

    loss_r2 = loss_Ab3 @ Ab3_r2  # shape (50)
    loss_A3 = np.outer(Ab3_A3, loss_Ab3)
    loss_b3 = loss_Ab3 * Ab3_b3  # shape (10)

    loss_Ab2 = loss_r2 * r2_Ab2  # shape (50)

    loss_r1 = loss_Ab2 @ Ab2_r1  # shape (200)
    loss_A2 = np.outer(Ab2_A2, loss_Ab2)
    loss_b2 = loss_Ab2 * Ab2_b2  # shape (50)

    loss_Ab1 = loss_r1 * r1_Ab1  # shape (200)

    # loss_x = loss_Ab1 @ Ab1_x  # shape (784)
    loss_A1 = np.outer(Ab1_A1, loss_Ab1)
    loss_b1 = loss_Ab1 * Ab1_b1  # shape (200)

    # update A3, b3, A2, b2, A1, b1
    # Adam

    delta_A3 += loss_A3
    delta_b3 += loss_b3
    delta_A2 += loss_A2
    delta_b2 += loss_b2
    delta_A1 += loss_A1
    delta_b1 += loss_b1


def update():
    global m_A3, m_b3, m_A2, m_b2, m_A1, m_b1, v_A3, v_b3, v_A2, v_b2, v_A1, v_b1, updateCount
    global A3, b3, A2, b2, A1, b1, delta_A3, delta_b3, delta_A2, delta_b2, delta_A1, delta_b1

    delta_A3 = delta_A3 / batchSize * np.random.randint(0, 2, delta_A3.shape)
    delta_b3 = delta_b3 / batchSize * np.random.randint(0, 2, delta_b3.shape)
    delta_A2 = delta_A2 / batchSize * np.random.randint(0, 2, delta_A2.shape)
    delta_b2 = delta_b2 / batchSize * np.random.randint(0, 2, delta_b2.shape)
    delta_A1 = delta_A1 / batchSize * np.random.randint(0, 2, delta_A1.shape)
    delta_b1 = delta_b1 / batchSize * np.random.randint(0, 2, delta_b1.shape)

    m_A3 = beta1 * m_A3 + (1 - beta1) * delta_A3
    v_A3 = beta2 * v_A3 + (1 - beta2) * delta_A3**2
    A3 -= learningRate * m_A3 / (np.sqrt(v_A3) + eps)

    m_b3 = beta1 * m_b3 + (1 - beta1) * delta_b3
    v_b3 = beta2 * v_b3 + (1 - beta2) * delta_b3**2
    b3 -= learningRate * m_b3 / (np.sqrt(v_b3) + eps)

    m_A2 = beta1 * m_A2 + (1 - beta1) * delta_A2
    v_A2 = beta2 * v_A2 + (1 - beta2) * delta_A2**2
    A2 -= learningRate * m_A2 / (np.sqrt(v_A2) + eps)

    m_b2 = beta1 * m_b2 + (1 - beta1) * delta_b2
    v_b2 = beta2 * v_b2 + (1 - beta2) * delta_b2**2
    b2 -= learningRate * m_b2 / (np.sqrt(v_b2) + eps)

    m_A1 = beta1 * m_A1 + (1 - beta1) * delta_A1
    v_A1 = beta2 * v_A1 + (1 - beta2) * delta_A1**2
    A1 -= learningRate * m_A1 / (np.sqrt(v_A1) + eps)

    m_b1 = beta1 * m_b1 + (1 - beta1) * delta_b1
    v_b1 = beta2 * v_b1 + (1 - beta2) * delta_b1**2
    b1 -= learningRate * m_b1 / (np.sqrt(v_b1) + eps)

    delta_A3 = np.zeros_like(delta_A3)
    delta_b3 = np.zeros_like(delta_b3)
    delta_A2 = np.zeros_like(delta_A2)
    delta_b2 = np.zeros_like(delta_b2)
    delta_A1 = np.zeros_like(delta_A1)
    delta_b1 = np.zeros_like(delta_b1)


def showPredict(allIndex):
    allData = np.array(rawTestX)[allIndex]
    result = forward(allData)
    predicted = np.argmax(result, axis=1)

    for i in range(len(allIndex)):
        plt.subplot(2, len(allIndex), i + 1)
        plt.imshow(allData[i].reshape(28, 28), cmap='gray')
        plt.title(f'{predicted[i]}')
        plt.axis('off')

    plt.show()


trainLoss = []
trainAccuracy = []
testLoss = []
testAccuracy = []


def test(epochIndex):
    length = len(testX)
    totalLoss = 0
    count = 0
    for i in range(length):
        x = testX[i]
        target = testTarget[i]
        y = forward(x)
        loss = nllLoss(y, target)
        predicted = np.argmax(y)
        if predicted == target:
            count += 1
        totalLoss += loss
    avgLoss = totalLoss / length
    accuracy = count / length
    testLoss.append(avgLoss)
    testAccuracy.append(accuracy)
    print(f'epoch: {epochIndex:2},               loss: {avgLoss:<8.2f} accuracy: {accuracy:<8.2f}', )


def train(epochIndex):
    global delta_A3, delta_b3, delta_A2, delta_b2, delta_A1, delta_b1
    count = 0

    for batchIndex in range(batchCount):
        dataStartIndex = batchSize * batchIndex

        for i in range(1, size):
            subprocessStartIndex = everySubprocessTask * (i - 1) + dataStartIndex
            subprocessEndIndex = subprocessStartIndex + everySubprocessTask
            subprocessTrainX = trainX[subprocessStartIndex:subprocessEndIndex]
            subprocessTrainTarget = trainTarget[subprocessStartIndex:subprocessEndIndex]

            # comm broadcast
            comm.send((A3, b3, A2, b2, A1, b1, learningRate, subprocessTrainX, subprocessTrainTarget), dest=i)

        delta_A3 = np.zeros_like(A3)
        delta_b3 = np.zeros_like(b3)
        delta_A2 = np.zeros_like(A2)
        delta_b2 = np.zeros_like(b2)
        delta_A1 = np.zeros_like(A1)
        delta_b1 = np.zeros_like(b1)

        for i in range(1, size):
            subDelta_A3, subDelta_b3, subDelta_A2, subDelta_b2, subDelta_A1, subDelta_b1, subCount = comm.recv(source=i)
            count += subCount

            delta_A3 += subDelta_A3
            delta_b3 += subDelta_b3
            delta_A2 += subDelta_A2
            delta_b2 += subDelta_b2
            delta_A1 += subDelta_A1
            delta_b1 += subDelta_b1

        delta_A3 /= size - 1
        delta_b3 /= size - 1
        delta_A2 /= size - 1
        delta_b2 /= size - 1
        delta_A1 /= size - 1
        delta_b1 /= size - 1

        update()

        if dataStartIndex % logInterval == 0 and dataStartIndex != 0:
            # avgLoss = totalLoss / logInterval
            accuracy = count / logInterval
            # totalLoss = 0
            count = 0

            print(f'epoch: {epochIndex:2}, index: {dataStartIndex:5}, accuracy: {accuracy:<8.2f}')


def plotLossAccuracy():
    # plot log loss
    plt.subplot(2, 1, 1)
    plt.plot(np.log(trainLoss), label='train')
    plt.plot(np.log(testLoss), label='test')
    plt.legend()
    plt.title('loss')

    plt.subplot(2, 1, 2)
    plt.plot(trainAccuracy, label='train')
    plt.plot(testAccuracy, label='test')
    plt.legend()
    plt.title('accuracy')

    # export
    plt.savefig('result.png')
    plt.show()


epochNumber = 2
batchCount = len(trainX) // batchSize
everySubprocessTask = batchSize // (size - 1)
allBatchCount = epochNumber * batchCount

if rank == 0:
    logInterval = 1000
    learningRate = 0.0001

    startTime = time.time()

    for i in range(epochNumber):
        train(i)
        test(i)
        learningRate *= 0.5

    endTime = time.time()
    timeCost = endTime - startTime
    print(f'time cost: {timeCost:.2f}s')

    # plotLossAccuracy()
    showPredict(range(10))

else:

    for _ in range(allBatchCount):
        delta_A3 = np.zeros_like(A3)
        delta_b3 = np.zeros_like(b3)
        delta_A2 = np.zeros_like(A2)
        delta_b2 = np.zeros_like(b2)
        delta_A1 = np.zeros_like(A1)
        delta_b1 = np.zeros_like(b1)

        # get data
        A3, b3, A2, b2, A1, b1, learningRate, subprocessTrainX, subprocessTrainTarget = comm.recv(source=0)

        subCount = 0
        for i in range(everySubprocessTask):
            x = subprocessTrainX[i]
            target = subprocessTrainTarget[i]
            y = forward(x)
            loss = nllLoss(y, target)
            backward(x, target)

            predicted = np.argmax(y)
            if predicted == target:
                subCount += 1

        comm.send((delta_A3, delta_b3, delta_A2, delta_b2, delta_A1, delta_b1, subCount), dest=0)
