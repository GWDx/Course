class Node {
public:
    Node* p;
    int rank;

    Node() {
        p = this;
        rank = 0;
    }
};

Node* findSet(Node* x) {
    if (x != x->p)
        x->p = findSet(x->p);
    return x->p;
}

void link(Node* x, Node* y) {
    if (x->rank > y->rank)
        y->p = x;
    else {
        x->p = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

void combine(Node* x, Node* y) {
    link(findSet(x), findSet(y));
}
