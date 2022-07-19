#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include <map>

#define INF 100

using namespace std;

string path;
vector<vector<int>> target(5, vector<int>(5));

// 做出的选择
class Option {
public:
    int x, y;          //下一步的位置
    string direction;  //移动的方向

    Option(int x, int y, string direction) {
        this->x = x;
        this->y = y;
        this->direction = direction;
    }
};

// 位置
class Position {
public:
    int x, y;

    Position(int x, int y) {
        this->x = x;
        this->y = y;
    }
    Position() {
        this->x = 0;
        this->y = 0;
    }
};

// 状态
class State {
public:
    vector<vector<int>> board;  // 棋盘
    string path;                // 路径
    Position position;          // 当前位置

    State(vector<vector<int>>& board, string path, Position position) : board(board), path(path), position(position) {}
};

// 根据输入状态，生成若干下一步状态
void genNextStates(State& inputState, vector<State>& nextStates) {
    vector<vector<int>>& board = inputState.board;
    string& path = inputState.path;
    int x, y;

    x = inputState.position.x;
    y = inputState.position.y;
    vector<Option> allOption;
    if (x != 0)
        allOption.push_back(Option(x - 1, y, "U"));
    else if (y == 2)
        allOption.push_back(Option(4, 2, "U"));

    if (x != 4)
        allOption.push_back(Option(x + 1, y, "D"));
    else if (y == 2)
        allOption.push_back(Option(0, 2, "D"));

    if (y != 0)
        allOption.push_back(Option(x, y - 1, "L"));
    else if (x == 2)
        allOption.push_back(Option(2, 4, "L"));

    if (y != 4)
        allOption.push_back(Option(x, y + 1, "R"));
    else if (x == 2)
        allOption.push_back(Option(2, 0, "R"));

    for (auto option : allOption)
        if (board[option.x][option.y] > 0) {
            State result(board, path + option.direction, Position(option.x, option.y));
            result.board[x][y] = result.board[option.x][option.y];
            result.board[option.x][option.y] = board[x][y];
            nextStates.push_back(result);
        }
}

// h1，返回棋盘中错位的星球数
int h1(vector<vector<int>>& board) {
    int count = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            int startValue = board[i][j];
            int targetValue = target[i][j];
            if (targetValue > 0 && startValue != targetValue)
                count++;
        }
    return count;
}

// 在可以穿过边界的条件下，x1 与 x2 的距离
int dist(int x1, int x2) {
    int abs1 = abs(x1 - x2);
    // x1 = 4, x2 = 1, abs2 = 2
    int abs2;
    if (x1 > x2)
        abs2 = 5 - x1 + x2;
    else
        abs2 = 5 - x2 + x1;
    int result = min(abs1, abs2);
    return result;
}

// 根据星球的 index 转换为 Position
vector<Position> allTargetPosition;

// h2，假设四条边界都可以穿过，返回所有星球 index 与目标星球 index 的横纵坐标距离之和
int h2(vector<vector<int>>& board) {
    vector<Position> allStartPosition(allTargetPosition.size());

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            int index = board[i][j];
            if (index > 0) {
                allStartPosition[index].x = i;
                allStartPosition[index].y = j;
            }
        }

    int diff = 0;
    for (int i = 0; i < allTargetPosition.size(); i++) {
        int startX = allStartPosition[i].x;
        int startY = allStartPosition[i].y;
        int targetX = allTargetPosition[i].x;
        int targetY = allTargetPosition[i].y;
        // if (startX != targetX || startY != targetY)
        //     diff += 1;
        diff += dist(startX, targetX) + dist(startY, targetY);
    }
    return diff;
}

Position getInitPosition(vector<vector<int>>& board) {
    int x, y;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == 0) {
                x = i;
                y = j;
                break;
            }
    return Position(x, y);
}

// 迭代 A*，使用启发函数 h2
string iterativeDeepeningAStarSearch2(vector<vector<int>> board) {
    int dLimit = h2(board);
    vector<string> result;
    while (dLimit < INF) {
        int nextDLimit = INF;
        vector<State> List;
        List.push_back(State(board, "", getInitPosition(board)));
        while (List.size() != 0) {
            State s = List.back();
            List.pop_back();
            int hs = h2(s.board);
            int ds = hs + s.path.size();  // 计算 f
            if (ds > dLimit)
                nextDLimit = min(nextDLimit, ds);  // 更新 nextDLimit
            else {
                if (hs == 0)
                    return s.path;
                vector<State> newStates;
                genNextStates(s, newStates);
                for (auto& newState : newStates)
                    List.push_back(newState);
            }
        }
        dLimit = nextDLimit;
    }
    return "FAIL";
}

// 标记启发函数值的状态，用于实现优先队列
class MarkedState {
public:
    int value;
    State state;

    MarkedState(int value, State state) : value{value}, state{state} {}
};

struct cmp {
    bool operator()(MarkedState& a, MarkedState& b) { return a.value > b.value; }
};

// A* 算法，使用启发函数 h2
string aStarSearch2(vector<vector<int>> board) {
    int h = h2(board);
    priority_queue<MarkedState, vector<MarkedState>, cmp> pq;
    pq.push(MarkedState(h, State(board, "", getInitPosition(board))));
    while (pq.size() != 0) {
        MarkedState s = pq.top();
        pq.pop();
        int f = s.value;
        vector<State> newStates;
        genNextStates(s.state, newStates);
        for (auto& newState : newStates) {
            int hs = h2(newState.board);
            if (hs == 0)
                return newState.path;
            int ds = hs + newState.path.size();
            pq.push(MarkedState(ds, newState));
        }
    }
    return "FAIL";
}

// 迭代 A*，使用启发函数 h1
string iterativeDeepeningAStarSearch1(vector<vector<int>> board) {
    int dLimit = h1(board);
    vector<string> result;
    while (dLimit < INF) {
        int nextDLimit = INF;
        vector<State> List;
        List.push_back(State(board, "", getInitPosition(board)));
        while (List.size() != 0) {
            State s = List.back();
            List.pop_back();
            int hs = h1(s.board);
            int ds = hs + s.path.size();
            if (ds > dLimit)
                nextDLimit = min(nextDLimit, ds);
            else {
                if (hs == 0)
                    return s.path;
                vector<State> newStates;
                genNextStates(s, newStates);
                for (auto& newState : newStates)
                    List.push_back(newState);
            }
        }
        dLimit = nextDLimit;
    }
    return "FAIL";
}

// A* 算法，使用启发函数 h1
string aStarSearch1(vector<vector<int>> board) {
    int h = h1(board);
    priority_queue<MarkedState, vector<MarkedState>, cmp> pq;
    pq.push(MarkedState(h, State(board, "", getInitPosition(board))));
    while (pq.size() != 0) {
        MarkedState s = pq.top();
        pq.pop();  // 弹出 f 最小的状态
        int f = s.value;
        vector<State> newStates;
        genNextStates(s.state, newStates);
        for (auto& newState : newStates) {
            int hs = h1(newState.board);

            if (hs == 0)
                return newState.path;
            int ds = hs + newState.path.size();
            pq.push(MarkedState(ds, newState));
        }
        // // 根据 prev 的状态和启发函数值计算 new 的启发函数值（优化失败）
        // for (auto& newState : newStates) {
        //     auto prevPosition = s.state.position;
        //     auto newPosition = newState.position;
        //     auto prevBoard = s.state.board;
        //     auto newBoard = newState.board;
        //     auto targetPrevValue = target[prevPosition.x][prevPosition.y];
        //     auto targetNewValue = target[newPosition.x][newPosition.y];
        //     auto newPrevValue = newBoard[prevPosition.x][prevPosition.y];
        //     auto prevNewValue = prevBoard[newPosition.x][newPosition.y];

        //     // int prevH = s.value - s.state.path.size();
        //     int delta = 0;

        //     // if (targetPrevValue > 0)
        //     if (targetPrevValue == newPrevValue)
        //         delta--;
        //     // if (targetNewValue > 0)
        //     if (targetNewValue == prevNewValue)
        //         delta++;

        //     // int ds = newH + newState.path.size();
        //     int ds = s.value + delta + 1;
        //     if (ds == newState.path.size())
        //         return newState.path;
        //     pq.push(MarkedState(ds, newState));
        // }
    }
    return "FAIL";
}

void A_h1(const vector<vector<int>>& board, const vector<vector<int>>& target) {
    ::target = target;
    path = aStarSearch1(board);
}

void A_h2(const vector<vector<int>>& board, const vector<vector<int>>& target) {
    ::target = target;
    allTargetPosition = vector<Position>(25);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            int index = target[i][j];
            if (index > 0) {
                allTargetPosition[index].x = i;
                allTargetPosition[index].y = j;
            }
        }
    path = aStarSearch2(board);
}

void IDA_h1(const vector<vector<int>>& board, const vector<vector<int>>& target) {
    ::target = target;
    path = iterativeDeepeningAStarSearch1(board);
}

void IDA_h2(const vector<vector<int>>& board, const vector<vector<int>>& target) {
    ::target = target;

    allTargetPosition = vector<Position>(25);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            int index = target[i][j];
            if (index > 0) {
                allTargetPosition[index].x = i;
                allTargetPosition[index].y = j;
            }
        }
    path = iterativeDeepeningAStarSearch2(board);
}

void eval(string algorithm, string inputFile, string targetFile) {
    // board
    auto startTime = chrono::high_resolution_clock::now();

    ifstream inStart(inputFile);
    vector<vector<int>> board(5, vector<int>(5));
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            inStart >> board[i][j];

    ifstream inTarget(targetFile);
    vector<vector<int>> tempTarget(5, vector<int>(5));
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            inTarget >> tempTarget[i][j];

    if (algorithm == "A_h1")
        A_h1(board, tempTarget);
    else if (algorithm == "A_h2")
        A_h2(board, tempTarget);
    else if (algorithm == "IDA_h1")
        IDA_h1(board, tempTarget);
    else if (algorithm == "IDA_h2")
        IDA_h2(board, tempTarget);

    // end
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    double seconds = duration.count() * 1e-6;

    string currentOutputFile = "output_" + algorithm + ".txt";
    ofstream out(currentOutputFile, ios::app);
    out << path << "," << seconds << endl;
    cout << path << "," << seconds << endl;
    out.close();
}

// ./a.out A_h1 input01.txt target01.txt
int main(int argc, char const* argv[]) {
    if (argc == 4) {
        string algorithm = argv[1];
        string inputFile = argv[2];
        string targetFile = argv[3];

        eval(algorithm, inputFile, targetFile);
    } else {
        cout << "./a.out A_h1 input01.txt target01.txt" << endl;
        vector<string> algorithms = {"A_h1", "A_h2", "IDA_h1", "IDA_h2"};

        for (auto& algorithm : algorithms)
            for (int i = 0; i <= 11; i++) {
                string indexStr = to_string(i);
                if (indexStr.size() == 1)
                    indexStr = "0" + indexStr;
                string inputFile = "data/input" + indexStr + ".txt";
                string targetFile = "data/target" + indexStr + ".txt";
                eval(algorithm, inputFile, targetFile);
            }
    }
    return 0;
}
