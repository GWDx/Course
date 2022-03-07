from cmath import inf


def getNextNum(thisSide, boat, otherSide, side):
    if side == False:
        ans = [thisSide, boat, otherSide]
    else:
        ans = [otherSide, boat, thisSide]
    return ans


history = []
cost = 0
minCost = inf
recordHistory = []


def dfs(num, step, side):
    global history, cost, minCost, recordHistory
    if [num, step, side] in history:
        return
    history.append([num, step, side])
    cost += 1

    if num[2] == [3, 3]:
        if minCost > cost:
            recordHistory = history[:]
            minCost = cost

    if side == False:
        [thisSide, boat, otherSide] = num
    else:
        [otherSide, boat, thisSide] = num

    if step == 1:
        # for i in range(2):
        if boat[0] > 0:
            nextThisSide = [thisSide[0] + 1, thisSide[1]]
            nextBoat = [boat[0] - 1, boat[1]]
            nextNum = getNextNum(nextThisSide, nextBoat, otherSide, side)
            dfs(nextNum, 1, side)
        if boat[1] > 0:
            nextThisSide = [thisSide[0], thisSide[1] + 1]
            nextBoat = [boat[0], boat[1] - 1]
            nextNum = getNextNum(nextThisSide, nextBoat, otherSide, side)
            dfs(nextNum, 1, side)
        dfs(num, 2, side)

    elif step == 2:
        if boat[0] + boat[1] < 2:
            if thisSide[0] > 0:
                nextThisSide = [thisSide[0] - 1, thisSide[1]]
                nextBoat = [boat[0] + 1, boat[1]]
                nextNum = getNextNum(nextThisSide, nextBoat, otherSide, side)
                dfs(nextNum, 2, side)
            if thisSide[1] > 0:
                nextThisSide = [thisSide[0], thisSide[1] - 1]
                nextBoat = [boat[0], boat[1] + 1]
                nextNum = getNextNum(nextThisSide, nextBoat, otherSide, side)
                dfs(nextNum, 2, side)
        if 0 < boat[0] + boat[1] <= 2 and thisSide[0] >= thisSide[1]:
            dfs(num, 3, side)
    elif step == 3:
        nextSide = not side
        dfs(num, 1, nextSide)

    history.pop()
    cost -= 1


def main():
    dfs([[3, 3], [0, 0], [0, 0]], 2, False)

    for item in recordHistory:
        print(f'{item[0]}	{item[1]}	{item[2]}')
    print('minCost=', minCost)


main()
