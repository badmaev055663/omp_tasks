#include "common.h"

#define SEQ 0
#define OMP 1

/* test one of dot_product versions (fixed vector size) */
double dot_product_test(int n, int opt)
{
    double t = 0.0;
    int *vec1 = (int*)malloc(sizeof(int) * n);
    int *vec2 = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < NUM_ITER; i++) {
        rand_fill_vec_int(vec1, n);
        rand_fill_vec_int(vec2, n);
        double t1 = omp_get_wtime();
        int res;
        switch (opt) {
            case SEQ:
            res = dot_product(vec1, vec2, n);
            break;
            case OMP:
            res = dot_product_omp(vec1, vec2, n);
            break;
        }
        double t2 = omp_get_wtime();
        t += (t2 - t1);
    }
    free(vec1);
    free(vec2);
    return t / NUM_ITER;
}

double dot_product_check(int n, int opt)
{
    int *vec1 = (int*)malloc(sizeof(int) * n);
    int *vec2 = (int*)malloc(sizeof(int) * n);

    for (int i = 0; i < 5; i++) {
        rand_fill_vec_int(vec1, n);
        rand_fill_vec_int(vec2, n);
        int res1 = dot_product(vec1, vec2, n);
        int res2 = dot_product_omp(vec1, vec2, n);
        if (res1 != res2) {
            printf("dot product check failed\n");
            free(vec1);
            free(vec2);
            return -1.0;
        }
    }
    free(vec1);
    free(vec2);
    return 0.0;
}


int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 500000;
    params.step = 500000;
    params.num_steps = 5;


    /*strcpy(params.file_name, "out/dot_product_data1");
    strcpy(params.label, "dot product seq");
    params.options = SEQ;
    bench(dot_product_test, &params);

    strcpy(params.file_name, "out/dot_product_data2");
    strcpy(params.label, "dot product omp");
    params.options = OMP;
    bench(dot_product_test, &params);*/
    strcpy(params.file_name, "out/dot_product_data");
    strcpy(params.label, "dot product omp");
    params.options = OMP;
    //thread_bench(dot_product_test, &params);
    thread_bench(dot_product_check, &params);


    return 0;
}