#include "FibonacciHeap.hpp"

List* FibonacciHeap::newList() {
    List* list = new List(this);
    lists.push_back(list);
    return list;
}

FibonacciHeap::FibonacciHeap() {
    n = 0;
    min = nullptr;
    rootList = new List(this);

    allFibonacciHeap.push_back(this);
}

void FibonacciHeap::insert(int key) {
    Node* x = new Node(this->rootList, key);
    x->degree = 0;
    x->p = nullptr;
    x->mark = false;
    if (this->min == nullptr) {
        rootList->add(x);
        this->min = x;
    } else {
        rootList->add(x);
        if (x->key < this->min->key)
            this->min = x;
    }
    this->n++;
}

FibonacciHeap* FibonacciHeap::unionWith(FibonacciHeap* H2) {
    FibonacciHeap* H1 = this;
    FibonacciHeap* H = new FibonacciHeap();
    H->min = H1->min;

    H->rootList->nodes.splice(H->rootList->nodes.end(), H1->rootList->nodes);
    H->rootList->nodes.splice(H->rootList->nodes.end(), H2->rootList->nodes);

    if (H1->min == nullptr or (H2->min != nullptr and H2->min->key < H1->min->key))
        H->min = H2->min;
    H->n = H1->n + H2->n;
    for (auto node : H->rootList->nodes)
        node->list = H->rootList;
    return H;
}

Node* FibonacciHeap::extractMin() {
    Node* z = H->min;
    if (z != nullptr) {
        if (z->child != nullptr)
            for (auto x : z->child->nodes) {
                H->rootList->add(x);
                x->p = nullptr;
            }
        H->min->remove();
        if (H->rootList->nodes.empty())
            H->min = nullptr;
        else {
            H->min = H->rootList->nodes.front();  // ? z.right
            _consolidate();
        }
        H->n--;
    }
    return z;
}

void FibonacciHeap::_consolidate() {
    vector<Node*> A(H->n + 1, nullptr);

    list<Node*> nodes = H->rootList->nodes;

    for (auto w : nodes) {
        Node* x = w;
        int d = x->degree;
        while (A[d] != nullptr) {
            Node* y = A[d];
            if (x->key > y->key) {
                Node* temp = x;
                x = y;
                y = temp;
            }
            _link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }
    H->min = nullptr;
    for (int i = 0; i <= H->n; i++) {
        if (A[i] != nullptr) {
            if (H->min == nullptr) {
                H->rootList->add(A[i]);
                H->min = A[i];
            } else {
                H->rootList->add(A[i]);
                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }
}

void FibonacciHeap::_link(Node* y, Node* x) {
    y->remove();
    x->child->add(y);
    y->p = x;
    x->degree++;
    y->mark = false;
}

void FibonacciHeap::decreaseKey(Node* x, int k) {
    if (k > x->key)
        cout << "error";
    x->key = k;
    Node* y = x->p;
    if (y != nullptr and x->key < y->key) {
        _cut(x, y);
        _cascadingCut(y);
    }
    if (x->key < H->min->key)
        H->min = x;
}

void FibonacciHeap::_cut(Node* x, Node* y) {
    x->remove();
    y->degree--;
    H->rootList->add(x);
    x->p = nullptr;
    x->mark = false;
}

void FibonacciHeap::_cascadingCut(Node* y) {
    Node* z = y->p;
    if (z != nullptr) {
        if (y->mark == false)
            y->mark = true;
        else {
            _cut(y, z);
            _cascadingCut(z);
        }
    }
}

void FibonacciHeap::deleteNode(Node* x) {
    int inf = 1000000;
    decreaseKey(x, -inf);
    extractMin();
}

void FibonacciHeap::minimumPrint() {
    cout << min->key << ",";
}

void FibonacciHeap::decreaseKeyPrint(int k, int key) {
    Node* x = search(k);
    decreaseKey(x, key);
    nodeByKey[k] = nullptr;
    nodeByKey[key] = x;
    cout << min->key << ",";
}

void FibonacciHeap::deletePrint(int k) {
    Node* x = search(k);
    deleteNode(x);
    cout << H->n << ",";
}

void FibonacciHeap::insertPrint(int key) {
    insert(key);
    cout << H->n << ",";
}

void FibonacciHeap::extractMinPrint() {
    Node* n = extractMin();
    cout << n->key << ",";
}

Node* FibonacciHeap::search(int key) {
    return nodeByKey[key];
}
