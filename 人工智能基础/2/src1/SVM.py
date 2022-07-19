import numpy as np
import cvxpy


class SupportVectorMachine:
    def __init__(self, C=1, kernel='Linear', epsilon=1e-4):
        self.C = C
        self.epsilon = epsilon
        self.kernel = kernel

        # Hint: 你可以在训练后保存这些参数用于预测
        # SV即Support Vector，表示支持向量，SV_alpha为优化问题解出的alpha值，
        # SV_label表示支持向量样本的标签。
        # self.SV = None
        # self.SV_alpha = None
        # self.SV_label = None

    def KERNEL(self, x1, x2, d=2, sigma=1):
        #d for Poly, sigma for Gauss
        if self.kernel == 'Gauss':
            K = np.exp(-(np.sum((x1 - x2.T)**2)) / (2 * sigma**2))
        elif self.kernel == 'Linear':
            K = np.dot(x1, x2)
        elif self.kernel == 'Poly':
            K = (np.dot(x1, x2) + 1)**d
        else:
            raise NotImplementedError()
        return K

    def fit(self, train_data, train_label):
        # TODO：实现软间隔 SVM 训练算法
        # train_data：训练数据，是 (N, 7) 的 numpy 二维数组，每一行为一个样本
        # train_label：训练数据标签，是 (N,) 的 numpy 数组，和 train_data 按行对应

        trainCount = train_data.shape[0]
        trainDim = train_data.shape[1]
        assert train_label.shape[0] == trainCount

        # max : sum(alpha_i) - sum(alpha_i * alpha_j * y_i * y_j * K(x_i, x_j)) / 2
        #   0 <= alpha_i <= C
        #   sum(alpha_i * y_i) = 0

        x = train_data
        # y is train_label to (1, trainCount)
        y = train_label.reshape(-1, 1)

        # 初始化约束条件
        alpha = cvxpy.Variable(trainCount)

        P = (y @ y.T) * self.KERNEL(x, x.T)

        objective = cvxpy.Maximize(cvxpy.sum(alpha) * 2 - cvxpy.quad_form(alpha, P))
        constraints = [alpha >= 0, alpha <= self.C, alpha @ train_label == 0]

        # 初始化问题
        problem = cvxpy.Problem(objective, constraints)
        problem.solve(solver='ECOS')

        # 获取解
        # w = sum(alpha_i * y_i * x_i)
        # b = y_i - w x_i, where alpha_i = 0
        alphaValue = alpha.value
        self.w = sum(alphaValue[i] * y[i] * x[i] for i in range(trainCount))

        allB = []
        for i in range(trainCount):
            if alphaValue[i] > 1e-5:
                allB.append(y[i] - self.w @ x[i])
        self.b = np.mean(allB)

    def predict(self, test_data):
        # TODO：实现软间隔 SVM 预测算法
        # train_data：测试数据，是 (M, 7) 的 numpy 二维数组，每一行为一个样本
        # 必须返回一个(M,)的numpy数组，对应每个输入预测的标签，取值为1或-1表示正负例

        # 初始化预测结果
        testCount = test_data.shape[0]
        testDim = test_data.shape[1]

        # 初始化预测结果
        predict = np.zeros(testCount)

        # 预测结果
        for i in range(testCount):
            predict[i] = self.w @ test_data[i] + self.b
            if predict[i] >= 0:
                predict[i] = 1
            else:
                predict[i] = -1
        print(predict)
        return predict
