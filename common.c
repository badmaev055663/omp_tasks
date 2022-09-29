#include "common.h"

void rand_fill_vec_int(int* vec, int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        vec[i] = RAND_OFFSET + rand() % MAX_RAND_VAL;
    }
}

void bench(double(*foo)(int, int), struct bench_params* params)
{
    FILE *fd;
    if ((fd = fopen(params->file_name, "w")) == NULL)
    {
        printf("failed to open file: %s\n", params->file_name);
        return;
    }
    int n = params->start_sz;
    fprintf(fd, "%s\n", params->label);
    for (int i = 0; i < params->num_steps; i++) {
        double t = foo(n, params->options);
        fprintf(fd, "%d %lf\n", n, t * 1000.0);
        n += params->step;
    }
    fclose(fd);
}

int find_min(int* vec, int n)
{
    int min = vec[0];
    for (int i = 1; i < n; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

/* unsafe */
int** alloc_sqr_mat(int n)
{
    int **res = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        res[i] = (int*)malloc(sizeof(int) * n);
    }
    return res;
}

void free_sqr_mat(int **mat, int n)
{
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}