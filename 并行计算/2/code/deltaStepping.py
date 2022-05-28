from math import ceil, inf
import numpy as np

# V = [0, 1, 2, 3, 4, 5, 6]
# E = [[0, 1, 3], [1, 2, 3], [2, 3, 1], [0, 3, 5], [0, 4, 3], [4, 5, 5], [0, 6, 3]]

# lenV = len(V)
# lenE = len(E)

with open('data.txt') as f:
    content = f.read()

firstLine = content.split('\n')[0]
firstLineSplit = firstLine.split(' ')
lenV, lenE = list(map(int, firstLineSplit))
otherLine = content.split('\n')[1:]

V = list(range(lenV))
E = []

for i in range(lenE):
    lineSplit = otherLine[i].split(' ')
    u, v, w = list(map(int, lineSplit))
    E.append((u, v, w))

adj = [{} for _ in range(lenV)]
for u, v, w in E:
    adj[u][v] = w
#     adj[v][u] = w

delta = 6

B = [set() for _ in range(lenV)]
for i in range(ceil(lenV / delta)):
    B[i] = set(V[i * delta:(i + 1) * delta])


def deltaStepping(s):
    global B, tent

    tent = [inf for _ in range(lenV)]

    relax(s, 0)
    while True:
        for i in range(len(B)):
            if B[i]:
                break
        R = set()
        while B[i]:
            req = findRequests(B[i], 'light')
            R = R.union(B[i])
            B[i] = set()
            relaxRequest(req)
        req = findRequests(R, 'heavy')
        relaxRequest(req)

        # print(B)
        # print(tent)
        flag = True
        for b in B:
            if b:
                flag = False
        if flag:
            break
    return tent


def findRequests(vv, kind):
    ans = []
    if kind == 'light':
        for v in vv:
            for w, c in adj[v].items():
                if c <= delta:
                    ans.append((w, tent[v] + c))
    elif kind == 'heavy':
        for v in vv:
            for w, c in adj[v].items():
                if c > delta:
                    ans.append((w, tent[v] + c))
    return ans


def relaxRequest(req):
    for (w, x) in req:
        relax(w, x)


def relax(w, x):
    global tent
    if x < tent[w]:
        # try:
        #     b = B[tent[w] // delta]
        #     b.remove(w)
        # except:
        #     pass
        if tent[w] // delta in B:
            b = B[tent[w] // delta]
            if w in b:
                b.remove(w)
        B[x // delta].update([w])
        tent[w] = x


result = deltaStepping(0)
for item in result:
    print(item)
