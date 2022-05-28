#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <unistd.h>

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
    if (rank == lastIndex)
        readMatrix(A, m, n, notZeroCount);

    ifstream in2("x.txt");
    if (!in2) {
        cout << "Error opening file" << endl;
        std::abort();
    }
    for (int i = 0; i < n; i++)
        in2 >> x[i];

    // send m, n, notZeroCount to all
    MPI_Bcast(&m, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    MPI_Bcast(&notZeroCount, 1, MPI_INT, lastIndex, MPI_COMM_WORLD);

    MPI_Win win;
    MPI_Aint size_of_win1, size_of_win2, size_of_win3;
    void* rowPtrAddress;
    void* colIndexAddress;
    void* valAddress;
    // send A.rowPtr, A.colIndex, A.val to all
    // MPI_Bcast(A.rowPtr, n + 1, MPI_INT, lastIndex, MPI_COMM_WORLD);
    // MPI_Bcast(A.colIndex, notZeroCount, MPI_INT, lastIndex, MPI_COMM_WORLD);
    // MPI_Bcast(A.val, notZeroCount, MPI_DOUBLE, lastIndex, MPI_COMM_WORLD);

    // 共享内存
    if (rank == lastIndex) {
        MPI_Win_allocate_shared((n + 1) * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &rowPtrAddress,
                                &win);
        MPI_Win_allocate_shared(notZeroCount * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD,
                                &colIndexAddress, &win);
        MPI_Win_allocate_shared(notZeroCount * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD,
                                &valAddress, &win);
    } else {
        int disp_unit1, disp_unit2, disp_unit3;
        MPI_Win_allocate_shared(0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &rowPtrAddress, &win);
        MPI_Win_shared_query(win, MPI_PROC_NULL, &size_of_win1, &disp_unit1, &rowPtrAddress);

        MPI_Win_allocate_shared(0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &colIndexAddress, &win);
        MPI_Win_shared_query(win, MPI_PROC_NULL, &size_of_win2, &disp_unit2, &colIndexAddress);

        MPI_Win_allocate_shared(0, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &valAddress, &win);
        MPI_Win_shared_query(win, MPI_PROC_NULL, &size_of_win3, &disp_unit3, &valAddress);
    }

    int* rowPtr = (int*)rowPtrAddress;
    int* colIndex = (int*)colIndexAddress;
    double* val = (double*)valAddress;

    if (rank == lastIndex) {
        memcpy(rowPtr, A.rowPtr, (n + 1) * sizeof(int));
        memcpy(colIndex, A.colIndex, notZeroCount * sizeof(int));
        memcpy(val, A.val, notZeroCount * sizeof(double));
    }
    // 同步各个进程
    MPI_Win_fence(0, win);

    auto startTime = std::chrono::high_resolution_clock::now();

    int start = rank * n / size;
    int end = (rank + 1) * n / size;
    if (rank == lastIndex)
        end = n;

    for (int i = start; i < end; i++) {
        y[i] = 0;
        for (int k = rowPtr[i]; k < rowPtr[i + 1]; k++)
            y[i] += val[k] * x[colIndex[k]];
    }

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
