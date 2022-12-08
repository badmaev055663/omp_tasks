#include "common.h"

#define OMP 1
#define OMP1 2
#define OMP2 3
#define OMP3 4


int dot_product_omp1(int *vec1, int *vec2, int n)
{
    int res = 0;
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int tmp = vec1[i] * vec2[i];    
        #pragma omp critical
        res += tmp;
    }
    return res;
}

int dot_product_omp2(int *vec1, int *vec2, int n)
{
    int res = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int tmp = vec1[i] * vec2[i];  
        omp_set_lock(&lock);
        res += tmp;
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    return res;
}

int dot_product_omp3(int *vec1, int *vec2, int n)
{
    int res = 0;
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int tmp = vec1[i] * vec2[i];    
        #pragma omp atomic
        res += tmp;
    }
    return res;
}

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
            case OMP1:
            res = dot_product_omp1(vec1, vec2, n);
            break;
            case OMP2:
            res = dot_product_omp2(vec1, vec2, n);
            break;
            case OMP3:
            res = dot_product_omp3(vec1, vec2, n);
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
        int res2 = dot_product_omp1(vec1, vec2, n);
        int res3 = dot_product_omp2(vec1, vec2, n);  
        int res4 = dot_product_omp3(vec1, vec2, n);
        if (res2 != res1 || res3 != res1 || res4 != res1) {
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
    params.start_sz = 100000;
    params.step = 100000;
    params.num_steps = 8;

    /*strcpy(params.file_name, "out/sync_cmp_data1");
    strcpy(params.label, "dot product critical");
    params.options = OMP1;
    bench(dot_product_test, &params);

    strcpy(params.file_name, "out/sync_cmp_data2");
    strcpy(params.label, "dot product lock");
    params.options = OMP2;
    bench(dot_product_test, &params);

    strcpy(params.file_name, "out/sync_cmp_data3");
    strcpy(params.label, "dot product atomic");
    params.options = OMP3;
    bench(dot_product_test, &params);

    strcpy(params.file_name, "out/sync_cmp_data4");
    strcpy(params.label, "dot product reduction");
    params.options = OMP;
    bench(dot_product_test, &params);*/
    strcpy(params.file_name, "out/sync_cmp_data");
    strcpy(params.label, "dot product crit");
    params.options = OMP1;
    thread_bench(dot_product_check, &params);

    return 0;
}