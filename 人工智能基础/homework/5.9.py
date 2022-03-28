def sameData(board, x, y, z):
    if board[x] == board[y] == board[z] and board[x] != 0:
        return True


def validStageCount(board, player):
    allTestIndex = [[0, 1, 2], [3, 4, 5], [6, 7, 8], [0, 3, 6], [1, 4, 7], [2, 5, 8], [0, 4, 8], [2, 4, 6]]
    for i in allTestIndex:
        if sameData(board, i[0], i[1], i[2]):
            return 1
    count = 0
    for i in range(9):
        if board[i] == 0:
            anotherPlayer = 3 - player
            board[i] = player
            count += validStageCount(board, anotherPlayer)
            board[i] = 0
    return count


def evalState(board):
    X = {1: 0, 2: 0}
    allTestIndex = [[0, 1, 2], [3, 4, 5], [6, 7, 8], [0, 3, 6], [1, 4, 7], [2, 5, 8], [0, 4, 8], [2, 4, 6]]
    for indexTuple in allTestIndex:
        line = [board[index] for index in indexTuple]
        for player in [1, 2]:
            if player not in line:
                anotherPlayer = 3 - player
                if anotherPlayer in line:
                    X[anotherPlayer] += 1
    ans = X[1] - X[2]
    return ans


def markDepth2():
    ans = []
    for i in [1, 2, 4, 5, 8]:
        board = [0 for i in range(9)]
        board[0] = 1
        board[i] = 2
        ans.append(evalState(board))
    print(ans)

    ans = []
    for i in [0, 3, 4, 6, 7]:
        board = [0 for i in range(9)]
        board[1] = 1
        board[i] = 2
        ans.append(evalState(board))
    print(ans)

    ans = []
    for i in [0, 1]:
        board = [0 for i in range(9)]
        board[4] = 1
        board[i] = 2
        ans.append(evalState(board))
    print(ans)


def main():
    board = [0 for i in range(9)]
    player = 1
    print(validStageCount(board, player))

    markDepth2()


main()
