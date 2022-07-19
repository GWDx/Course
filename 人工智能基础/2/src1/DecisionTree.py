import numpy as np


# 决策树的结点
class Node:
    def __init__(self, feature=None, children=None):
        self.feature = feature
        if children == None:
            self.children = {}


class DecisionTree:
    def splitDataset(self, dataset, index):
        allDataRange = set(dataset[:, index])
        result = {}
        for value in allDataRange:
            result[value] = dataset[dataset[:, index] == value]
        return result

    def fit(self, train_features, train_labels):
        # TODO: 实现决策树学习算法.
        # train_features 是维度为 (训练样本数, 属性数) 的 numpy 数组
        # train_labels 是维度为 (训练样本数, ) 的numpy数组

        trainCount = train_features.shape[0]
        attributeCount = train_features.shape[1]
        assert trainCount == train_labels.shape[0]

        self.valid = [True] * attributeCount + [False]

        combinedData = np.hstack((train_features, train_labels.reshape(-1, 1)))

        node = self.generateTree(combinedData)
        self.root = node

    def generateTree(self, combinedData):
        count = combinedData.shape[0]
        attributeCount = combinedData.shape[1] - 1

        train_features = combinedData[:, :-1]
        train_labels = combinedData[:, -1]

        # 生成结点 node
        node = Node()

        node.leafValue = np.argmax(np.bincount(train_labels))

        if np.all(train_labels == train_labels[0]):
            # node.leafValue = train_labels[0]
            return node
        if not (True in self.valid) or len(np.unique(train_labels)) == 1:
            return node

        # 选择最优的划分属性
        bestAttributeIndex = self.chooseBestAttributeIndex(train_features, train_labels)
        # print("best attribute: ", bestAttributeIndex)
        node.feature = bestAttributeIndex

        self.valid[bestAttributeIndex] = False

        # 划分属性值的范围
        split = self.splitDataset(combinedData, bestAttributeIndex)

        for key, value in split.items():
            # 递归生成子结点
            childNode = self.generateTree(value)
            node.children[key] = childNode

        self.valid[bestAttributeIndex] = True
        return node

    def calculateEntropy(self, train_labels):
        entropy = 0
        for i in range(len(np.unique(train_labels))):
            p = np.sum(train_labels == i) / len(train_labels)
            entropy += -p * np.log2(p + 1e-10)
        return entropy

    def chooseBestAttributeIndex(self, train_features, train_labels):
        trainCount = train_features.shape[0]
        attributeCount = train_features.shape[1]
        assert trainCount == train_labels.shape[0]

        # ID3 算法
        bestInfoGain = 0
        bestAttributeIndex = 0
        initEntropy = self.calculateEntropy(train_labels)

        for i in range(attributeCount):
            if self.valid[i] == False:
                continue

            # 划分属性值的范围
            split = self.splitDataset(train_features, i)

            # 计算信息增益
            entropy = 0
            for key, value in split.items():
                entropy += len(value) / trainCount * self.calculateEntropy(value[:, i])

            infoGain = initEntropy - entropy
            if infoGain > bestInfoGain:
                bestInfoGain = infoGain
                bestAttributeIndex = i

        return bestAttributeIndex

    def predict(self, test_features):
        # TODO: 实现决策树预测.
        # test_features是维度为(测试样本数,属性数)的 numpy 数组
        # 该函数需要返回预测标签，返回一个维度为(测试样本数, )的numpy数组

        # 初始化预测标签
        predict_labels = np.zeros(test_features.shape[0])

        # 遍历测试样本
        for i in range(test_features.shape[0]):
            node = self.root
            while node.feature is not None:
                featureValue = test_features[i][node.feature]
                if featureValue in node.children:
                    childNode = node.children[featureValue]
                    node = childNode
                else:
                    break
            predict_labels[i] = node.leafValue

        print(predict_labels)
        return predict_labels


# treenode: [attr, feat[attr] == 1, feat[attr] == 2, feat[attr] == 3]
