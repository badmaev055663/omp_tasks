#include "common.h"

void rand_fill_vec_int(int* vec, int n)
{
    srand(clock());
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

int dot_product(int *vec1, int *vec2, int n)
{
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += vec1[i] * vec2[i];
    }
    return res;
}

int dot_product_omp(int *vec1, int *vec2, int n)
{
    int res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int i = 0; i < n; i++) {    
        res += vec1[i] * vec2[i];
    }
    return res;
}

int** alloc_sqr_mat(int n)
{
    int **res = (int**)malloc(sizeof(int*) * n);
    int i;
    if (!res)
        return NULL;
    for (i = 0; i < n; i++) {
        res[i] = (int*)malloc(sizeof(int) * n);
        if (!res[i])
            goto clean;
    }
    return res;

clean:
    for (int j = 0; j < i; j++)
        free(res[j]);
    free(res);
    return NULL;
}

void free_sqr_mat(int **mat, int n)
{
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void thread_bench(double(*foo)(int, int), struct bench_params* params)
{
    char buffer1[8];
    char buffer2[8];
    int len1 = strlen(params->file_name);
    int len2 = strlen(params->label);
    for (int t = 2; t <= MAX_THREADS; t *= 2) {
        omp_set_num_threads(t);
        sprintf(buffer1, " %dT", t);
        sprintf(buffer2, " %dT", t);
        strcat(params->file_name, buffer1);
        strcat(params->label, buffer2);
        bench(foo, params);
        params->file_name[len1] = '\0';
        params->label[len2] = '\0';

    }
}