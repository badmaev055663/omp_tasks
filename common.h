#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include "omp.h"

/* assume we have <= 32 threads */
#define MAX_THREADS 32

/* iterations for obtaining average */
#define NUM_ITER 10

/* prevent integer overflow */
#define MAX_RAND_VAL 1000
#define RAND_OFFSET -500

#define MAX_STR_LEN 32

struct bench_params {
    char file_name[MAX_STR_LEN];
    char label[MAX_STR_LEN];
    int start_sz;
    int step;
    int num_steps;
    int options;
};

void rand_fill_vec_int(int* vec, int n);
void bench(double(*foo)(int, int), struct bench_params *params);
int find_min(int* vec, int n);

int** alloc_sqr_mat(int n);
void free_sqr_mat(int **mat, int n);