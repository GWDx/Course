#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <queue>
#include <fstream>
#include <chrono>

using namespace std;

#define INF 100000000

void log(string s) {
    cerr << "\033[1;34m" << s << "\033[0m" << endl;
}

int unknownCount = 0;

class AddExpression;

class UnknownBool {
public:
    int value;
    bool determined;
    int id;
    vector<AddExpression*> appeared;
    bool valueRange[2];

    UnknownBool() {
        value = 0;
        determined = false;
        id = unknownCount++;
        valueRange[0] = true;
        valueRange[1] = true;
    }
};

int expressionCount = 0;

enum ExpressionType { Equal, LessEqual, GreaterEqual };

class AddExpression {
public:
    vector<UnknownBool*> left;
    int right;
    int id;
    ExpressionType type;
    int undeterminedCount;  // 未确定的变量个数
    int determinedSum = 0;  // 确定的和
    double weight;

    AddExpression(vector<UnknownBool*> left, int right) : left(left), right(right) {
        for (auto& unknown : left)
            unknown->appeared.push_back(this);

        undeterminedCount = (int)left.size();
        id = expressionCount++;
    }

    bool evaluate() {
        // assert(undeterminedCount == 0);
        int sum = 0;
        for (auto& unknown : left)
            sum += unknown->value;
        if (type == ExpressionType::Equal)
            return sum == right;
        else if (type == ExpressionType::LessEqual)
            return sum <= right;
        else
            return sum >= right;
    }

    bool checkPossible(int value) {
        int ans = value + determinedSum;
        if (type == ExpressionType::Equal)
            return ans <= right and right <= ans + undeterminedCount;
        else if (type == ExpressionType::LessEqual)
            return ans <= right;
        else
            return right <= ans + undeterminedCount;
    }
};

double factorial(int n) {
    double result = 1;
    for (int i = 1; i <= n; i++)
        result *= i;
    return result;
}

double binomial(int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}

class AddEqual : public AddExpression {
public:
    AddEqual(vector<UnknownBool*> left, int right) : AddExpression(left, right) {
        type = ExpressionType::Equal;
        double probablity = binomial(left.size(), right) / pow(2, left.size());
        weight = probablity * sqrt(left.size());
    }
};

class AddLessEqual : public AddExpression {
public:
    AddLessEqual(vector<UnknownBool*> left, int right) : AddExpression(left, right) {
        type = ExpressionType::LessEqual;
        double probablity = 0;
        for (int i = 0; i <= right; i++)
            probablity += binomial(left.size(), i) / pow(2, left.size());
        weight = probablity * sqrt(left.size());
    }
};

class AddGreaterEqual : public AddExpression {
public:
    AddGreaterEqual(vector<UnknownBool*> left, int right) : AddExpression(left, right) {
        type = ExpressionType::GreaterEqual;
        double probablity = 0;
        for (int i = right; i <= (int)left.size(); i++)
            probablity += binomial(left.size(), i) / pow(2, left.size());
        weight = probablity * sqrt(left.size());
    }
};

class Solver {
public:
    vector<UnknownBool*> unknowns;
    vector<AddExpression*> expressions;
    vector<vector<int>> allSolution;
    int allSolutionCount;

    int evalCount = 1;    // -1 means no limit
    int recordCount = 1;  // -1 means no limit
    bool orderOption = true;
    bool mrvOption = false;
    bool expressionCheckOption = true;

    Solver(vector<UnknownBool*> unknowns, vector<AddExpression*> equalExpressions)
        : unknowns(unknowns), expressions(equalExpressions) {
        unknownNumber = unknowns.size();
    }

    void solve();

    void geneticSolve();
    int fitness(vector<int>& solution);

private:
    int unknownNumber;

    void order();
    void simpleVisit(int unknownIndex);
    void edgeCheckVisit(int unknownIndex);
};

void Solver::solve() {
    allSolutionCount = 0;
    allSolution.clear();

    if (orderOption)
        order();

    if (expressionCheckOption)
        edgeCheckVisit(0);
    else
        simpleVisit(0);

    unknownCount = 0;
    expressionCount = 0;
}

vector<int> associateExpressionCount;  // id -> count

void Solver::order() {
    sort(unknowns.begin(), unknowns.end(),
         [](UnknownBool* a, UnknownBool* b) { return a->appeared.size() > b->appeared.size(); });

    if (mrvOption) {
        associateExpressionCount = vector<int>(unknownNumber);
        for (int i = 0; i < unknownNumber; i++)
            associateExpressionCount[unknowns[i]->id] = unknowns[i]->appeared.size();

        for (int i = 0; i < unknownNumber; i++) {
            // try to remove unknowns[i]
            // decrease associateExpressionCount
            auto unknown = unknowns[i];
            auto& appeared = unknown->appeared;
            for (auto& expression : appeared) {
                expression->undeterminedCount--;
                if (expression->undeterminedCount == 0)
                    for (auto& u : expression->left)
                        associateExpressionCount[u->id]--;
            }
            sort(unknowns.begin() + i + 1, unknowns.end(), [](UnknownBool* a, UnknownBool* b) {
                return associateExpressionCount[a->id] > associateExpressionCount[b->id];
            });
        }
        // reset expression->undeterminedCount
        for (auto& expression : expressions)
            expression->undeterminedCount = expression->left.size();
    }
    for (auto& unknown : unknowns)
        sort(unknown->appeared.begin(), unknown->appeared.end(),
             [](AddExpression* a, AddExpression* b) { return a->weight < b->weight; });
}

void Solver::simpleVisit(int unknownIndex) {
    // 设置未知数的值及其他属性
    auto unknown = unknowns[unknownIndex];
    // if (unknown->determined)
    //     return;
    unknown->determined = true;

    for (auto& expression : unknown->appeared)
        expression->undeterminedCount--;

    // for (int value = 0; value <= 1; value++) {
    for (int value = 1; value >= 0; value--) {
        unknown->value = value;
        bool correct = true;
        // 判断各个约束是否满足
        for (auto& expression : unknown->appeared)
            if (expression->undeterminedCount == 0) {
                bool expressionValue = expression->evaluate();
                if (!expressionValue) {
                    correct = false;
                    break;
                }
            }
        if (!correct)
            continue;

        // 如果满足，且全部访问完毕，则记录结果，否则访问下一个
        if (unknownIndex + 1 == unknownNumber) {
            if ((int)allSolution.size() != recordCount) {
                vector<int> solution(unknownNumber);
                for (auto& unknown : unknowns)
                    solution[unknown->id] = unknown->value;
                allSolution.push_back(solution);
            }
            allSolutionCount++;
        } else
            simpleVisit(unknownIndex + 1);

        if (allSolutionCount == evalCount)
            return;
    }
    unknown->determined = false;

    for (auto& expression : unknown->appeared)
        expression->undeterminedCount++;
}

void Solver::edgeCheckVisit(int unknownIndex) {
    auto unknown = unknowns[unknownIndex];
    // if (unknown->determined)
    //     return;
    unknown->determined = true;

    for (auto& expression : unknown->appeared)
        expression->undeterminedCount--;

    // for (int value = 0; value <= 1; value++) {
    for (int value = 1; value >= 0; value--) {
        unknown->value = value;
        bool correct = true;

        for (auto& expression : unknown->appeared) {
            if (!expression->checkPossible(value)) {
                correct = false;
                break;
            }
        }

        if (!correct)
            continue;

        for (auto& expression : unknown->appeared)
            expression->determinedSum += value;

        if (unknownIndex + 1 == unknownNumber) {
            if ((int)allSolution.size() != recordCount) {
                vector<int> solution(unknownNumber);
                for (auto& unknown : unknowns)
                    solution[unknown->id] = unknown->value;
                allSolution.push_back(solution);
            }
            allSolutionCount++;
        } else
            edgeCheckVisit(unknownIndex + 1);

        if (allSolutionCount == evalCount)
            return;

        for (auto& expression : unknown->appeared)
            expression->determinedSum -= value;
    }
    unknown->determined = false;

    for (auto& expression : unknown->appeared)
        expression->undeterminedCount++;
}

class IndividualFitness {
public:
    vector<int> solution;
    int fitness;

    IndividualFitness(vector<int>& solution, int fitness) : solution(solution), fitness(fitness) {}
    IndividualFitness() {
        solution = vector<int>(0);
        fitness = 0;
    }
};

// fitness = sum(expression->evaluate)
// fitness == expressions.size() means correct
int Solver::fitness(vector<int>& individual) {
    for (auto& unknown : unknowns)
        unknown->value = individual[unknown->id];
    int fitness = 0;
    for (auto& expression : expressions)
        if (expression->evaluate())
            fitness++;
    return fitness;
}

// genetic algorithm
void Solver::geneticSolve() {
    allSolutionCount = 0;
    allSolution.clear();
    int i;

    int populationSize = 100;
    int generationCount = 30000;
    int mutationRate = 20;
    int crossoverRate = 80;

    vector<vector<int>> population(populationSize);
    for (i = 0; i < populationSize; i++) {
        vector<int> individual(unknownNumber);
        for (int j = 0; j < unknownNumber; j++)
            individual[j] = rand() % 2;
        population[i] = individual;
    }

    vector<IndividualFitness> fitnesses(populationSize);
    priority_queue<IndividualFitness, vector<IndividualFitness>, greater<IndividualFitness>> pq;
    for (i = 0; i < populationSize; i++)
        fitnesses[i] = IndividualFitness(population[i], fitness(population[i]));

    for (i = 0; i < generationCount; i++) {
        sort(fitnesses.begin(), fitnesses.end(),
             [](IndividualFitness a, IndividualFitness b) { return a.fitness > b.fitness; });

        if (fitnesses[0].fitness == (int)expressions.size())
            break;

        vector<vector<int>> newPopulation(populationSize);
        for (int j = 0; j < populationSize; j++) {
            if (rand() % 100 < crossoverRate) {
                int parent1 = rand() % populationSize;
                int parent2 = rand() % populationSize;
                int crossoverPoint = rand() % unknownNumber;
                vector<int> child(unknownNumber);
                for (int k = 0; k < crossoverPoint; k++)
                    child[k] = population[parent1][k];
                for (int k = crossoverPoint; k < unknownNumber; k++)
                    child[k] = population[parent2][k];
                newPopulation[j] = child;
            } else
                newPopulation[j] = population[j];
        }

        for (int j = 0; j < populationSize; j++)
            if (rand() % 100 < mutationRate) {
                int mutationPoint = rand() % unknownNumber;
                newPopulation[j][mutationPoint] = 1 - newPopulation[j][mutationPoint];
            }

        vector<IndividualFitness> newFitnesses(populationSize);
        for (int j = 0; j < populationSize; j++)
            newFitnesses[j] = IndividualFitness(newPopulation[j], fitness(newPopulation[j]));

        fitnesses = newFitnesses;
    }

    sort(fitnesses.begin(), fitnesses.end(),
         [](IndividualFitness a, IndividualFitness b) { return a.fitness > b.fitness; });

    if (fitnesses[0].fitness == (int)expressions.size()) {
        log("solution found in generation " + to_string(i));
        allSolutionCount = 1;
        allSolution.push_back(fitnesses[0].solution);
    } else
        log("genetic algorithm failed");

    cout << fitnesses[0].fitness << " / " << expressions.size() << endl;
}

void simpleTest() {
    auto a = new UnknownBool();
    auto b = new UnknownBool();
    auto c = new UnknownBool();
    auto d = new UnknownBool();
    vector<UnknownBool*> allUnknown = {a, b, c, d};

    auto e1 = new AddGreaterEqual({a, b}, 1);
    auto e2 = new AddLessEqual({b, c}, 1);
    auto e3 = new AddEqual({a, d}, 1);
    vector<AddExpression*> allExpression = {e1, e2, e3};

    Solver solver(allUnknown, allExpression);

    solver.geneticSolve();

    for (auto& solution : solver.allSolution) {
        for (auto& val : solution)
            cout << val << " ";
        cout << "\n";
    }

    solver.evalCount = -1;
    solver.recordCount = -1;
    solver.solve();

    log("allSolutionCount: " + to_string(solver.allSolutionCount));

    for (auto& solution : solver.allSolution) {
        for (auto& val : solution)
            cout << val << " ";
        cout << "\n";
    }
    cout << "\n";

    for (auto& unknown : allUnknown)
        delete unknown;
    for (auto& expression : allExpression)
        delete expression;
}

void task(int taskIndex, vector<string> workerLevel, int everydayNeed, vector<pair<int, int>> avoidWorkTogether) {
    int day, worker;
    int workerCount = workerLevel.size();

    vector<vector<UnknownBool*>> unknowns(7, vector<UnknownBool*>(workerCount));
    for (day = 0; day < 7; day++)
        for (worker = 0; worker < workerCount; worker++)
            unknowns[day][worker] = new UnknownBool();

    // 每天至少要有 everydayNeed 个人值班
    vector<AddExpression*> allExpression;
    for (day = 0; day < 7; day++) {
        vector<UnknownBool*> values;
        for (worker = 0; worker < workerCount; worker++)
            values.push_back(unknowns[day][worker]);
        allExpression.push_back(new AddGreaterEqual(values, everydayNeed));
    }

    // 每个工人每周必须休息 2 天或 2 天以上
    // 每个工人工作日之和小于等于 5
    for (worker = 0; worker < workerCount; worker++) {
        vector<UnknownBool*> values;
        for (day = 0; day < 7; day++)
            values.push_back(unknowns[day][worker]);
        allExpression.push_back(new AddLessEqual(values, 5));
    }

    // 每个工人每周不可以连续休息 3 天（不考虑跨周情况）
    // 每个工人连续三天 add > 0
    for (worker = 0; worker < workerCount; worker++)
        for (day = 0; day < 5; day++) {
            vector<UnknownBool*> values = {unknowns[day][worker], unknowns[day + 1][worker], unknowns[day + 2][worker]};
            allExpression.push_back(new AddGreaterEqual(values, 1));
        }

    // 每天至少要有一名级别为 senior 的工人值班
    for (day = 0; day < 7; day++) {
        vector<UnknownBool*> values;
        for (worker = 0; worker < workerCount; worker++)
            if (workerLevel[worker] == "senior")
                values.push_back(unknowns[day][worker]);
        allExpression.push_back(new AddGreaterEqual(values, 1));
    }

    // 工人 2 不想和工人 4 一起工作
    for (auto& pair : avoidWorkTogether)
        for (day = 0; day < 7; day++) {
            int worker1 = pair.first - 1;
            int worker2 = pair.second - 1;
            vector<UnknownBool*> values = {unknowns[day][worker1], unknowns[day][worker2]};
            allExpression.push_back(new AddLessEqual(values, 1));
        }

    vector<UnknownBool*> allUnknown;
    for (day = 0; day < 7; day++)
        for (worker = 0; worker < workerCount; worker++)
            allUnknown.push_back(unknowns[day][worker]);

    // Solver
    Solver solver(allUnknown, allExpression);

    solver.geneticSolve();
    for (auto& solution : solver.allSolution) {
        for (day = 0; day < 7; day++) {
            for (worker = 0; worker < workerCount; worker++)
                cout << solution[day * workerCount + worker] << " ";
            cout << "\n";
        }
        cout << "\n";
    }

    solver.evalCount = 10000000;
    solver.recordCount = 1;
    solver.orderOption = false;
    solver.mrvOption = false;
    solver.expressionCheckOption = false;

    auto start = chrono::high_resolution_clock::now();
    solver.solve();

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    double time = duration.count() / 1000000.0;
    int allSolutionCount = solver.allSolutionCount;
    log(to_string(allSolutionCount) + " solutions found in " + to_string(time) + " seconds.");

    for (auto& solution : solver.allSolution) {
        for (day = 0; day < 7; day++) {
            for (worker = 0; worker < workerCount; worker++)
                cout << solution[day * workerCount + worker] << " ";
            cout << "\n";
        }
        cout << "\n";
    }
    cout << "\n";

    string outputFileName = "output" + to_string(taskIndex) + ".txt";
    ofstream outputFile(outputFileName);

    for (auto& solution : solver.allSolution) {
        for (day = 0; day < 7; day++) {
            for (worker = 0; worker < workerCount; worker++)
                if (solution[day * workerCount + worker] == 1)
                    outputFile << worker + 1 << " ";
            outputFile << "\n";
        }
        outputFile << "\n";
    }

    outputFile.close();
    for (auto& unknown : allUnknown)
        delete unknown;
    for (auto& expression : allExpression)
        delete expression;
}

// ./main output.txt
int main(int argc, char const* argv[]) {
    simpleTest();

    // 工人1:senior
    // 工人2:junior
    // 工人3:junior
    // 工人4:senior
    // 工人5:junior
    // 工人 2 不想和工人 4 一起工作
    task(0, {"senior", "junior", "junior", "senior", "junior"}, 3, {{2, 4}});

    // 工人1:senior
    // 工人2:senior
    // 工人3:junior
    // 工人4:junior
    // 工人5:junior
    // 工人6:junior
    // 工人7:junior
    // 需要每天安排至少 4 个人值班，其中工人 1 和工人 4，工人 2 和工人 3，工人 3 和工人 6 不想在同一天工作。

    task(1, {"senior", "senior", "junior", "junior", "junior", "junior", "junior"}, 4, {{1, 4}, {2, 3}, {3, 6}});

    // 工人1:senior
    // 工人2:senior
    // 工人3:junior
    // 工人4:junior
    // 工人5:junior
    // 工人6:junior
    // 工人7:junior
    // 工人8:senior
    // 工人9:junior
    // 工人10:senior
    // 需要每天安排至少 5 个人值班，其中工人 1 和工人 5，工人 2 和工人 6，工人 8 和工人 10 不想在同一天工作
    task(2, {"senior", "senior", "junior", "junior", "junior", "junior", "junior", "senior", "junior", "senior"}, 5,
         {{1, 5}, {2, 6}, {8, 10}});

    return 0;
}
