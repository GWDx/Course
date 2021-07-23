import numpy
import random
import itertools
from functools import cmp_to_key

from sympy import gcd
from sympy.ntheory import factorint
from sympy.ntheory.factor_ import primeomega
from sympy.ntheory.modular import crt


# 压平矩阵
def flatten(l):
    return list(itertools.chain.from_iterable(l))


# 去除重复元素并排序
def union(l):
    return sorted(list(set(l)))


# 返回 x 的素因数
def primeFactors(x):
    ans = factorint(x)
    return ans.keys()


# range(lcm) 中没有被 partA 覆盖的数
def remainRangeLCM(ansM, partA):
    length = len(partA)
    partM = ansM[:length]
    lcm = 360
    result = []
    for x in range(lcm):
        notMod = True
        for i in range(length):
            if x % partM[i] == partA[i]:
                notMod = False
                break
        if notMod:
            result.append(x)
    return result


def remainLength(ansM, partA):
    return len(remainRangeLCM(ansM, partA))


# remainRangeLCM(ansM, partA) 模 ansM[length] 中的众数
def remainModCommonest(ansM, partA):
    length = len(partA)
    remained = remainRangeLCM(ansM, partA)
    remainMod = [i % ansM[length] for i in remained]
    bincounts = numpy.bincount(remainMod)
    ans = numpy.where(bincounts == bincounts.max())[0].tolist()   # 众数
    return ans


# 产生长度为 n 的 partA 的列表
def generateAllPartA(ansM, n):
    if n == 3:
        return [[0, 0, 0]]
    allPartA = generateAllPartA(ansM, n-1)
    all = []
    for partA in allPartA:
        remainCommonests = remainModCommonest(ansM, partA)
        for nextPartA in remainCommonests:
            all.append(partA + [nextPartA])
    ans = random.sample(all, min(len(all), 5))
    return ans


# 产生 allAnsA
def generateAllAnsA(ansM):
    allAnsA = []
    while allAnsA == []:
        for i in generateAllPartA(ansM, 18):
            if remainLength(ansM, i) == 0:
                allAnsA.append(i)
    return allAnsA


def main():
    fibonacci = [0, 1]
    for i in range(2, 400):
        fibonacci.append(fibonacci[i-1] + fibonacci[i-2])

    # 确定 ansM 和 ansP
    allMRange = []
    for i in range(4):
        for j in range(3):
            for k in range(2):
                allMRange.append(2 ** i * 3 ** j * 5 ** k)
    allMRange.sort()

    mRange = allMRange[2:]
    pRange = union(flatten([primeFactors(fibonacci[i]) for i in mRange]))

    mToP = {}
    for p in pRange:
        for m in mRange:
            if fibonacci[m] % p == 0 and m not in mToP:
                mToP[m] = p
                break

    selectedM = [3, 4, 5, 8, 9, 10, 15, 18, 20, 24, 30, 36, 40, 45, 60, 72, 90, 120]
    ansM = sorted(selectedM, key=cmp_to_key(lambda x, y: primeomega(x)-primeomega(y)))
    ansP = [mToP[m] for m in ansM]

    # 计算 ansA
    allAnsA = generateAllAnsA(ansM)
    unnormalizedAnsA = allAnsA[0]
    # unnormalizedAnsA = [0, 0, 0, 7, 7, 14, 2, 1, 13, 11, 4, 14, 10, 23, 23, 31, 22, 43]

    ansA = [(unnormalizedAnsA[i] - 1) % ansM[i] + 1 for i in range(len(ansM))]
    print('ansP =', ansP)
    print('ansM =', ansM)
    print('ansA =', ansA)

    # 计算 A0 和 A1
    fib1 = []
    fib2 = []
    for i in range(len(ansP)):
        fib1.append(fibonacci[ansM[i]-ansA[i]])
        fib2.append(fibonacci[ansM[i]-ansA[i]+1])
    A0 = crt(ansP, fib1)[0]
    A1 = crt(ansP, fib2)[0]
    if min(A0, A1) <= max(ansP) or gcd(A0, A1) != 1:
        raise RuntimeError('error')
    print('A0 =', A0)
    print('A1 =', A1)


main()
