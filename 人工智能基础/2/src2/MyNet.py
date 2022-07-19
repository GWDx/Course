import torch
import torchvision

import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

from torchvision import transforms

# device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
device = torch.device("cpu")


class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        ########################################################################
        #这里需要写MyNet的卷积层、池化层和全连接层

        # input: 3x32x32
        # 1: kernel conv2D 24, 9
        # 2: maxpooling
        # 3: kernel conv2D 32, 3
        # 4: maxpooling
        # 6: linear 108
        # 7: linear 72
        # 8: linear 10
        # use relu as activation function

        self.conv1 = nn.Conv2d(3, 24, 9)
        self.pool1 = nn.MaxPool2d(2)
        self.conv2 = nn.Conv2d(24, 32, 3)
        self.pool2 = nn.MaxPool2d(2)
        self.linear1 = nn.Linear(800, 108)
        self.linear2 = nn.Linear(108, 72)
        self.linear3 = nn.Linear(72, 10)

    def forward(self, x):
        ########################################################################
        #这里需要写MyNet的前向传播

        x = self.pool1(F.relu(self.conv1(x)))
        x = self.pool2(F.relu(self.conv2(x)))
        x = x.view(-1, 800)
        x = F.relu(self.linear1(x))
        x = F.relu(self.linear2(x))
        x = self.linear3(x)
        return x


def train(net, train_loader, optimizer, n_epochs, loss_function):
    net.train()
    for epoch in range(n_epochs):
        for step, (inputs, labels) in enumerate(train_loader, start=0):
            # get the inputs; data is a list of [inputs, labels]
            inputs, labels = inputs.to(device), labels.to(device)

            ########################################################################
            #计算loss并进行反向传播

            outputs = net(inputs)
            loss = loss_function(outputs, labels)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            ########################################################################

            if step % 100 == 0:
                print('Train Epoch: {}/{} [{}/{}]\tLoss: {:.6f}'.format(epoch, n_epochs, step * len(inputs),
                                                                        len(train_loader.dataset), loss.item()))

    print('Finished Training')
    save_path = './MyNet.pth'
    torch.save(net.state_dict(), save_path)


def test(net, test_loader, loss_function):
    net.eval()
    test_loss = 0.
    num_correct = 0  # correct 的个数
    with torch.no_grad():
        for inputs, labels in test_loader:
            inputs, labels = inputs.to(device), labels.to(device)
        ########################################################################
        #需要计算测试集的loss和accuracy

        for inputs, labels in test_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = net(inputs)
            loss = loss_function(outputs, labels)
            test_loss += loss
            _, predicted = torch.max(outputs.data, 1)
            num_correct += (predicted == labels).sum().item()

        accuracy = num_correct / len(test_loader.dataset)
        test_loss = test_loss / len(test_loader)

        ########################################################################
        print("Test set: Average loss: {:.4f}\t Acc {:.2f}".format(test_loss.item(), accuracy))


if __name__ == '__main__':
    n_epochs = 5
    train_batch_size = 128
    test_batch_size = 5000
    learning_rate = 5e-4

    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.247, 0.243, 0.261))])

    # 50000张训练图片
    train_set = torchvision.datasets.CIFAR10(root='./data', train=True, download=False, transform=transform)
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=train_batch_size, shuffle=True, num_workers=0)

    # 10000张验证图片
    test_set = torchvision.datasets.CIFAR10(root='./data', train=False, download=False, transform=transform)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=test_batch_size, shuffle=False, num_workers=0)

    net = MyNet()

    # 自己设定优化器和损失函数

    #######################################################################

    optimizer = optim.Adam(net.parameters(), lr=learning_rate)
    loss_function = nn.CrossEntropyLoss()

    train(net, train_loader, optimizer, n_epochs, loss_function)
    test(net, test_loader, loss_function)
