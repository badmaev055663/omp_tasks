#include "common.h"

void rand_fill_vec_int(int* vec, int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        vec[i] = RAND_OFFSET + rand() % MAX_RAND_VAL;
    }
}

void bench(double(*foo)(int, int), struct bench_params params)
{
    int n = params.start_sz;
    for (int i = 0; i < params.num_steps; i++) {
        double t = foo(n, params.options);
        printf("data size: %d\n", n);
        printf("wall clock time (ms): %.5lf\n", t * 1000.0);
        n += params.step;
    }
}