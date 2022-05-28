#include <gmpxx.h>
#include <omp.h>
#include <iostream>

#define endN 10000       // 计算 100 位只需设为 100
#define precision 40000  // 计算 100 位只需设为 400
#define kernelNumberMax 10

int main(int argc, char** argv) {
    int i, j;
    mpf_set_default_prec(precision);
    mpf_t ans;

    mpf_init(ans);
    mpf_set_ui(ans, 1);

    if (argc != 2)
        return 1;
    mpf_t partReciprocalFactorial[kernelNumberMax], partAns[kernelNumberMax];
    int kernelNumber = atoi(argv[1]);
    auto start = omp_get_wtime();

#pragma omp parallel num_threads(kernelNumber)
    {
        int i = omp_get_thread_num();
        mpf_init(partReciprocalFactorial[i]);
        mpf_set_ui(partReciprocalFactorial[i], 1);
        mpf_init(partAns[i]);
        mpf_set_ui(partAns[i], 0);

        for (int j = endN / kernelNumber * i + 1; j <= endN / kernelNumber * (i + 1); j++) {
            mpf_class mpfI = j;
            mpf_div(partReciprocalFactorial[i], partReciprocalFactorial[i], mpfI.get_mpf_t());
            mpf_add(partAns[i], partAns[i], partReciprocalFactorial[i]);
        }
    }

    mpf_t prevReciprocalFactorial, temp;
    mpf_init(prevReciprocalFactorial);
    mpf_set_ui(prevReciprocalFactorial, 1);
    mpf_init(temp);

    for (i = 0; i < kernelNumber; i++) {
        mpf_mul(temp, partAns[i], prevReciprocalFactorial);
        mpf_add(ans, ans, temp);
        mpf_mul(prevReciprocalFactorial, prevReciprocalFactorial, partReciprocalFactorial[i]);
    }
    auto end = omp_get_wtime();

    gmp_printf("%0.100Ff\n", ans);

    std::cout << "Time: " << end - start << std::endl;
}
