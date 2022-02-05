#include <fstream>
#include <iostream>
#include "FibonacciHeap.cpp"

#define FOR(i, l, r) for (i = l; i <= r; i++)

FibonacciHeap* H1;
FibonacciHeap* H2;
FibonacciHeap* H3;
FibonacciHeap* H4;
FibonacciHeap* H5;

void step1();
void step2();
void step3();
void step4();
void step5();
void step6();
void step7();

void printTime(void (*step)(), const char* HName, const char* stepName) {
    cout << HName << '\n';
    cerr << stepName << ":\t";

    clock_t start = clock();
    step();
    clock_t end = clock();
    clock_t time = end - start;

    cout << '\n';
    cerr << time << '\n';
}

int main() {
    H1 = new FibonacciHeap;
    H2 = new FibonacciHeap;
    H3 = new FibonacciHeap;
    H4 = new FibonacciHeap;

    step1();
    printTime(step2, "H1", "step2");
    printTime(step3, "H2", "step3");
    printTime(step4, "H3", "step4");
    printTime(step5, "H4", "step5");
    step6();
    printTime(step7, "H5", "step7");

    for (auto i : allNode)
        delete i;
    for (auto i : allList)
        delete i;
    for (auto i : allFibonacciHeap)
        delete i;

    return 0;
}

void step1() {
    int key[600], i;

    std::ifstream inputFile("../input/2_1_input.txt");
    if (!inputFile.is_open())
        exit(1);

    FOR (i, 1, 500)
        inputFile >> key[i];
    inputFile.close();

    FOR (i, 1, 50)
        H1->insert(key[i]);
    FOR (i, 51, 150)
        H2->insert(key[i]);
    FOR (i, 151, 300)
        H3->insert(key[i]);
    FOR (i, 301, 500)
        H4->insert(key[i]);
}

void step2() {
    H1->insertPrint(249);
    H1->insertPrint(830);
    H1->minimumPrint();
    H1->deletePrint(127);
    H1->deletePrint(141);
    H1->minimumPrint();
    H1->decreaseKeyPrint(75, 61);
    H1->decreaseKeyPrint(198, 169);
    H1->extractMinPrint();
    H1->extractMinPrint();
}

void step3() {
    H2->insertPrint(816);
    H2->minimumPrint();
    H2->insertPrint(345);
    H2->extractMinPrint();
    H2->deletePrint(504);
    H2->deletePrint(203);
    H2->decreaseKeyPrint(296, 87);
    H2->decreaseKeyPrint(278, 258);
    H2->minimumPrint();
    H2->extractMinPrint();
}

void step4() {
    H3->extractMinPrint();
    H3->minimumPrint();
    H3->insertPrint(262);
    H3->extractMinPrint();
    H3->insertPrint(832);
    H3->minimumPrint();
    H3->deletePrint(134);
    H3->deletePrint(177);
    H3->decreaseKeyPrint(617, 360);
    H3->decreaseKeyPrint(889, 353);
}

void step5() {
    H4->minimumPrint();
    H4->deletePrint(708);
    H4->insertPrint(281);
    H4->insertPrint(347);
    H4->minimumPrint();
    H4->deletePrint(415);
    H4->extractMinPrint();
    H4->decreaseKeyPrint(620, 354);
    H4->decreaseKeyPrint(410, 80);
    H4->extractMinPrint();
}

void step6() {
    H5 = H1->unionWith(H2)->unionWith(H3)->unionWith(H4);
}

void step7() {
    H5->extractMinPrint();
    H5->minimumPrint();
    H5->deletePrint(800);
    H5->insertPrint(267);
    H5->insertPrint(351);
    H5->extractMinPrint();
    H5->decreaseKeyPrint(478, 444);
    H5->decreaseKeyPrint(559, 456);
    H5->minimumPrint();
    H5->deletePrint(929);
}
