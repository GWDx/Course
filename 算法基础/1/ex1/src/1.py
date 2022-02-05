import sys


def printOptimalParens(s, i, j):
    if i == j:
        print(' A' + str(i), end='')
    else:
        print('(', end='')
        printOptimalParens(s, i, s[i][j])
        printOptimalParens(s, s[i][j] + 1, j)
        print(')', end='')


def matrixChainOrder(p):
    n = len(p) - 1
    m = [[0 for i in range(n + 1)] for j in range(n + 1)]
    s = [[0 for i in range(n + 1)] for j in range(n + 1)]
    for i in range(1, n + 1):
        m[i][i] = 0
    for l in range(2, n + 1):
        for i in range(1, n - l + 2):
            j = i + l - 1
            m[i][j] = float("inf")
            for k in range(i, j):
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j]
                if q < m[i][j]:
                    m[i][j] = q
                    s[i][j] = k
    return m, s


def main(epoch):
    n = int(input())
    raw = input().strip(' ').split(' ')
    p = [int(i) for i in raw]
    # n = 6
    # p = [30, 35, 15, 5, 10, 20, 25]
    for i in range(epoch):
        m, s = matrixChainOrder(p)
    print(m[1][n])
    printOptimalParens(s, 1, n)
    print()


if len(sys.argv) == 1:
    main(1)
else:
    main(int(sys.argv[1]))
