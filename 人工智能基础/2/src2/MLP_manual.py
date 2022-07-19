import torch
import numpy as np
from matplotlib import pyplot as plt


def softmax(x):
    e_x = np.exp(x)
    return e_x / e_x.sum()


# 设置随机种子,保证结果的可复现性
np.random.seed(1)

W1 = np.random.randn(10, 10)
W2 = np.random.randn(10, 8)
W3 = np.random.randn(8, 8)
W4 = np.random.randn(8, 4)

b1 = np.random.randn(10)
b2 = np.random.randn(8)
b3 = np.random.randn(8)
b4 = np.random.randn(4)


class MLP:
    def __init__(self):
        # layer size = [10, 8, 8, 4]
        # 初始化所需参数
        self.W1 = W1.copy()
        self.W2 = W2.copy()
        self.W3 = W3.copy()
        self.W4 = W4.copy()

        self.b1 = b1.copy()
        self.b2 = b2.copy()
        self.b3 = b3.copy()
        self.b4 = b4.copy()

    def forward(self, x):
        # 前向传播
        h1 = np.tanh(x @ self.W1 + self.b1)
        h2 = np.tanh(h1 @ self.W2 + self.b2)
        h3 = np.tanh(h2 @ self.W3 + self.b3)
        y = softmax(h3 @ self.W4 + self.b4)

        self.x, self.h1, self.h2, self.h3, self.y = x, h1, h2, h3, y
        return y

    def backward(self, label):  # 自行确定参数表
        # 反向传播

        # l' s4'
        x, h1, h2, h3, y = self.x, self.h1, self.h2, self.h3, self.y
        W1, W2, W3, W4 = self.W1, self.W2, self.W3, self.W4
        l_s4 = y - label  # shape (4)

        L_b4 = l_s4  # shape (4)
        L_W4 = np.outer(h3, L_b4)  # shape (8, 4)

        L_b3 = W4 @ L_b4 * (1 - h3**2)
        L_W3 = np.outer(h2, L_b3)

        L_b2 = W3 @ L_b3 * (1 - h2**2)
        L_W2 = np.outer(h1, L_b2)

        L_b1 = W2 @ L_b2 * (1 - h1**2)
        L_W1 = np.outer(x, L_b1)

        self.grad_W1, self.grad_W2, self.grad_W3, self.grad_W4 = L_W1, L_W2, L_W3, L_W4
        self.grad_b1, self.grad_b2, self.grad_b3, self.grad_b4 = L_b1, L_b2, L_b3, L_b4


def train(mlp: MLP, epochs, lr, inputs, labels):
    # mlp: 传入实例化的MLP模型
    # epochs: 训练轮数
    # lr: 学习率
    # inputs: 生成的随机数据
    # labels: 生成的one-hot标签
    allLoss = []

    for epoch in range(epochs):
        totalLoss = 0
        totalCount = 0
        for x, label in zip(inputs, labels):
            # 前向传播
            y = mlp.forward(x)

            # 计算 loss，使用 Cross Entropy Loss
            loss = -np.log(np.sum(y * label))
            totalLoss += loss

            # if correct
            if np.argmax(y) == np.argmax(label):
                totalCount += 1

            # 反向传播
            mlp.backward(label)

            # 更新参数
            mlp.W1 -= lr * mlp.grad_W1
            mlp.W2 -= lr * mlp.grad_W2
            mlp.W3 -= lr * mlp.grad_W3
            mlp.W4 -= lr * mlp.grad_W4

            mlp.b1 -= lr * mlp.grad_b1
            mlp.b2 -= lr * mlp.grad_b2
            mlp.b3 -= lr * mlp.grad_b3
            mlp.b4 -= lr * mlp.grad_b4

        avgLoss = totalLoss / len(inputs)
        accuracy = totalCount / len(inputs)
        print(f'epoch {epoch + 1:3}\tloss: {avgLoss:<20}\taccuracy: {accuracy}')
        allLoss.append(avgLoss)

    print('W1:', mlp.W1)
    print('W2:', mlp.W2)
    print('W3:', mlp.W3)
    print('W4:', mlp.W4)

    print('b1:', mlp.b1)
    print('b2:', mlp.b2)
    print('b3:', mlp.b3)
    print('b4:', mlp.b4)
    return np.array(allLoss)


class torchNet:
    def __init__(self):
        # self.W1 = torch from numpy W1
        self.W1 = torch.from_numpy(W1).double().requires_grad_()
        self.W2 = torch.from_numpy(W2).double().requires_grad_()
        self.W3 = torch.from_numpy(W3).double().requires_grad_()
        self.W4 = torch.from_numpy(W4).double().requires_grad_()

        self.b1 = torch.from_numpy(b1).double().requires_grad_()
        self.b2 = torch.from_numpy(b2).double().requires_grad_()
        self.b3 = torch.from_numpy(b3).double().requires_grad_()
        self.b4 = torch.from_numpy(b4).double().requires_grad_()

    def forward(self, x):
        h1 = torch.tanh(x @ self.W1 + self.b1)
        h2 = torch.tanh(h1 @ self.W2 + self.b2)
        h3 = torch.tanh(h2 @ self.W3 + self.b3)
        y = torch.softmax(h3 @ self.W4 + self.b4, dim=0)

        self.y = y
        return y

    def backward(self, loss):
        # use autograd to compute the backward pass
        loss.backward()
        self.grad_W1 = self.W1.grad
        self.grad_W2 = self.W2.grad
        self.grad_W3 = self.W3.grad
        self.grad_W4 = self.W4.grad

        self.grad_b1 = self.b1.grad
        self.grad_b2 = self.b2.grad
        self.grad_b3 = self.b3.grad
        self.grad_b4 = self.b4.grad


def torchTrain(epochs, lr, inputs, labels):
    # train torchNet
    allLoss = []
    net = torchNet()
    optimizer = torch.optim.SGD([net.W1, net.W2, net.W3, net.W4, net.b1, net.b2, net.b3, net.b4], lr=lr)

    for epoch in range(epochs):
        totalLoss = 0
        for x, label in zip(inputs, labels):
            x = torch.tensor(x).double()
            label = torch.tensor(label).double()

            # 前向传播
            y = net.forward(x)

            # 计算 loss
            loss = -torch.log(torch.dot(y, label))
            totalLoss += loss

            # 反向传播
            optimizer.zero_grad()
            net.backward(loss)
            optimizer.step()

        avgLoss = totalLoss / len(inputs)
        # print(f'epoch {epoch + 1:3}   loss: {avgLoss}')
        allLoss.append(avgLoss.item())

    return np.array(allLoss)


if __name__ == '__main__':
    mlp = MLP()
    # 生成数据
    inputs = np.random.randn(100, 10)

    # 生成one-hot标签
    labels = np.eye(4)[np.random.randint(0, 4, size=(1, 100))].reshape(100, 4)

    epochs = 100
    lr = 0.01

    # 训练
    allMlpLoss = train(mlp, epochs, lr, inputs, labels)
    allTorchLoss = torchTrain(epochs, lr, inputs, labels)

    # 绘制 loss 曲线，用 o 表示点
    plt.figure(figsize=(10, 6), dpi=200)
    plt.plot(allMlpLoss, 'o-', markersize=4)
    plt.xlabel('epoch')
    plt.ylabel('loss')
    # plt.title('损失随训练轮数的变化')
    plt.savefig('loss.png')
    # plt.show()

    # avgLoss
    avgDiffAbsLoss = np.abs(allMlpLoss - allTorchLoss).mean()
    print(f'avg abs diff loss between mlp and torch: {avgDiffAbsLoss}')
