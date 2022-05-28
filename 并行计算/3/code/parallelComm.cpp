#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

const int N = 10000005;

class SparseMatrix {
public:
    int rows;
    int rowPtr[N];
    int colIndex[N];
    double val[N];

    SparseMatrix() {
        rows = 0;
        for (int i = 0; i < N; i++)
            rowPtr[i] = 0;
    }
};
struct RawStruct {
    int row, col;
    double val;
};

RawStruct raw[N];

void readMatrix(SparseMatrix& A, int& m, int& n, int& notZeroCount) {
    ifstream in("matrix.mtx");
    if (!in) {
        cout << "Error opening file" << endl;
        std::abort();
    }

    // read matrix
    string line;
    std::stringstream ss;

    while (getline(in, line))
        if (line[0] != '%')
            ss << line << " ";

    in.close();

    ss >> m >> n >> notZeroCount;

    for (int i = 0; i < notZeroCount; i++) {
        int row, col;
        double val;
        ss >> row >> col >> val;
        raw[i].row = row - 1;
        raw[i].col = col - 1;
        raw[i].val = val;
    }

    // sort by row
    std::sort(raw, raw + notZeroCount, [](const RawStruct& a, const RawStruct& b) { return a.row < b.row; });

    vector<int> rowCount(m, 0);
    for (int i = 0; i < notZeroCount; i++)
        rowCount[raw[i].row]++;

    A.rows = n;
    for (int i = 0; i < n; i++)
        A.rowPtr[i + 1] = A.rowPtr[i] + rowCount[i];

    for (int i = 0; i < notZeroCount; i++) {
        A.colIndex[i] = raw[i].col;
        A.val[i] = raw[i].val;
    }
}

double y[N];
int x[N];
SparseMatrix A;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lastIndex = size - 1;

    int m, n, notZeroCount;
    // if (rank == lastIndex)
    readMatrix(A, m, n, notZeroCount);

    ifstream in2("x.txt");
    if (!in2) {
        cout << "Error opening file" << endl;
        std::abort();
    }
    for (int i = 0; i < n; i++)
        in2 >> x[i];

    // start timing

    // send m, n, notZeroCount to all
    MPI_Bcast(&m, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    MPI_Bcast(&notZeroCount, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);

    auto startTime = std::chrono::high_resolution_clock::now();

    // send A.rowPtr, A.colIndex, A.val to all
    // MPI_Bcast(A.rowPtr, n + 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    // MPI_Bcast(A.colIndex, notZeroCount, MPI_INT, lastIndex, MPI_COMM_WORLD);
    // MPI_Bcast(A.val, notZeroCount, MPI_DOUBLE, lastIndex, MPI_COMM_WORLD);
    if (rank == lastIndex)
        for (int i = 0; i < lastIndex; i++) {
            int start = i * n / size;
            int end = (i + 1) * n / size;
            MPI_Send(&A.rowPtr[start], end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            int kStart = A.rowPtr[start];
            int kEnd = A.rowPtr[end + 1];

            MPI_Send(&A.colIndex[kStart], kEnd - kStart, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&A.val[kStart], kEnd - kStart, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
        }
    else {
        int start = rank * n / size;
        int end = (rank + 1) * n / size;
        MPI_Recv(&A.rowPtr[start], end - start, MPI_INT, lastIndex, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int kStart = A.rowPtr[start];
        int kEnd = A.rowPtr[end + 1];

        MPI_Recv(&A.colIndex[kStart], kEnd - kStart, MPI_INT, lastIndex, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&A.val[kStart], kEnd - kStart, MPI_DOUBLE, lastIndex, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int start = rank * n / size;
    int end = (rank + 1) * n / size;
    if (rank == lastIndex)
        end = n;

    for (int i = start; i < end; i++)
        y[i] = 0;
    for (int i = start; i < end; i++)
        for (int k = A.rowPtr[i]; k < A.rowPtr[i + 1]; k++)
            y[i] += A.val[k] * x[A.colIndex[k]];

    // end timing
    if (rank != lastIndex)
        MPI_Send(y + start, end - start, MPI_DOUBLE, lastIndex, 3, MPI_COMM_WORLD);
    else
        for (int i = 0; i < lastIndex; i++) {
            int start = i * n / size;
            int end = (i + 1) * n / size;
            MPI_Recv(y + start, end - start, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cerr << rank << "/" << size << ": " << elapsed.count() << "s" << std::endl;

    if (rank == lastIndex) {
        for (int i = 0; i < n; i++)
            cout << y[i] << "\n";
    }

    MPI_Finalize();
}
