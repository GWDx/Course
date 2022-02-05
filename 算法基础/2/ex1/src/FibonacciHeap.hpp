#include <iostream>
#include <list>
#include <map>
#include <vector>

class Node;
class List;
class FibonacciHeap;

using std::cerr;
using std::cout;
using std::list;
using std::vector;

std::map<int, Node*> nodeByKey;
vector<Node*> allNode;
vector<List*> allList;
vector<FibonacciHeap*> allFibonacciHeap;

class Node {
public:
    int key;      // 键值
    int degree;   // 度
    Node* p;      // 父结点
    List* child;  // 孩子链表
    bool mark;    // 标记
    List* list;   // 所在链表

    Node(List* list, int key);

    void remove();  // 从所在链表移除
};

class List {
public:
    list<Node*> nodes;    // 包含的结点
    FibonacciHeap* heap;  // 所在的堆

    List(FibonacciHeap* heap);

    void add(Node* x);  // 插入结点 x
};

class FibonacciHeap {
public:
    int n;                // 结点数
    Node* min;            // 最小结点
    vector<List*> lists;  // 所有非根链表
    List* rootList;       // 根链表
    FibonacciHeap* H = this;

    FibonacciHeap();

    List* newList();        // 新建非根链表
    Node* search(int key);  // 按键值查找结点

    void insert(int key);  // 插入结点

    FibonacciHeap* unionWith(FibonacciHeap* H2);  // 与 H2 合并

    Node* extractMin();  // 弹出最小值
    void _consolidate();
    void _link(Node* y, Node* x);

    void decreaseKey(Node* x, int key);  // 减小键值
    void _cut(Node* x, Node* y);
    void _cascadingCut(Node* y);

    void deleteNode(Node* x);  // 删除结点

    void minimumPrint();
    void decreaseKeyPrint(int k, int key);
    void deletePrint(int k);
    void insertPrint(int key);
    void extractMinPrint();
};

Node::Node(List* list, int key) {
    this->key = key;
    this->list = list;
    this->degree = 0;
    this->child = list->heap->newList();
    this->p = nullptr;
    this->mark = false;
    nodeByKey[key] = this;

    allNode.push_back(this);
}

void Node::remove() {
    this->list->nodes.remove(this);
}

List::List(FibonacciHeap* heap) {
    this->heap = heap;
    allList.push_back(this);
}

void List::add(Node* x) {
    this->nodes.push_back(x);
    x->list = this;
}
