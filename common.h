#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include "omp.h"

/* assume we have <= 32 cores */
#define MAX_THREADS 32

/* iterations for obtaining average */
#define NUM_ITER 5

/* prevent integer overflow */
#define MAX_RAND_VAL 1000
#define RAND_OFFSET -500

struct bench_params {
    int start_sz;
    int step;
    int num_steps;
    int options;
};

void rand_fill_vec_int(int* vec, int n);

void bench(double(*foo)(int, int), struct bench_params params);