#include "common.h"

#define SEQ 0
#define OMP 1

/* omp 3.1+ */
int find_min_omp(int* vec, int n)
{
    int min_val = INT_MAX;
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < n; i++) {
        if (min_val >= vec[i])
            min_val = vec[i];
    }
    return min_val;
}

/* test one of find_min versions (fixed vector size) */
double find_min_test(int n, int opt)
{
    double t = 0.0;
    int *vec = (int*)malloc(sizeof(int) * n);

    for (int i = 0; i < NUM_ITER; i++) {
        rand_fill_vec_int(vec, n);
        double t1 = omp_get_wtime();
        int res;
        switch (opt) {
            case SEQ:
            res = find_min(vec, n);
            break;
            break;
            case OMP:
            res = find_min_omp(vec, n);
            break;
        }
        double t2 = omp_get_wtime();
        t += (t2 - t1);
    }
    free(vec);
    return t / NUM_ITER;
}


double find_min_check(int n, int opt)
{
    int *vec = (int*)malloc(sizeof(int) * n);

    for (int i = 0; i < 5; i++) {
        rand_fill_vec_int(vec, n);
        int res1 = find_min(vec, n);
        int res2 = find_min_omp(vec, n);
        if (res1 != res2) {
            printf("find min check failed\n");
            free(vec);
            return -1.0;
        }
    }
    free(vec);
    return 0.0;
}


int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 500000;
    params.step = 500000;
    params.num_steps = 5;

    /*params.options = SEQ;
    strcpy(params.file_name, "out/find_min_data1");
    strcpy(params.label, "find min seq");
    bench(find_min_test, &params);
    params.options = OMP;
    strcpy(params.file_name, "out/find_min_data2");
    strcpy(params.label, "find min omp");
    bench(find_min_test, &params);*/
    

    params.options = OMP;
    strcpy(params.file_name, "out/find_min_data");
    strcpy(params.label, "find min");
    //thread_bench(find_min_test, &params);
    thread_bench(find_min_check, &params);
 
    return 0;
}