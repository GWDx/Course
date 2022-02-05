import sys


def allStringLCS(b, c, X, m, n):
    allString = [[set() for i in range(n + 1)] for j in range(m + 1)]
    for i in range(0, m + 1):
        allString[i][0] = set([''])
    for j in range(0, n + 1):
        allString[0][j] = set([''])

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if b[i][j] == 'LeftUp':
                for item in allString[i - 1][j - 1]:
                    allString[i][j].add(item + X[i - 1])
            if c[i][j] == c[i - 1][j]:
                allString[i][j] = allString[i][j].union(allString[i - 1][j])
            if c[i][j] == c[i][j - 1]:
                allString[i][j] = allString[i][j].union(allString[i][j - 1])
    return sorted(allString[m][n])


def LCSLength(X, Y):
    m = len(X)
    n = len(Y)
    b = [[0 for i in range(n + 1)] for j in range(m + 1)]
    c = [[0 for i in range(n + 1)] for j in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if X[i - 1] == Y[j - 1]:
                c[i][j] = c[i - 1][j - 1] + 1
                # assert c[i][j] >= c[i - 1][j] and c[i][j] >= c[i][j - 1]
                b[i][j] = 'LeftUp'
            elif c[i - 1][j] >= c[i][j - 1]:
                c[i][j] = c[i - 1][j]
                b[i][j] = 'Up'
            else:
                c[i][j] = c[i][j - 1]
                b[i][j] = 'Left'
    return c, b


def main(epoch):
    input()
    X = input()
    Y = input()
    # X = 'AABBCD'
    # Y = 'ABCCDD'
    for i in range(epoch):
        m = len(X)
        n = len(Y)
        c, b = LCSLength(X, Y)
        # print(c[m][n])
        allLCS = allStringLCS(b, c, X, m, n)

    print(len(allLCS))
    for i in allLCS:
        print(i)


if len(sys.argv) == 1:
    main(1)
else:
    main(int(sys.argv[1]))
