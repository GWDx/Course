import numpy as np


def compandor(data, m):
    # ans=sgn(data)*ln(1+m abs(data))/ln(1+m)
    ans = np.sign(data) * np.log(1 + m * np.abs(data)) / np.log(1 + m)
    return ans


def classify(data):
    ans = np.round(data * 8 // 1 + 8)
    np.place(ans, ans == 16, 15)
    return ans.astype(np.int32)
