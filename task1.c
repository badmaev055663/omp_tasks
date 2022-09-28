#include "common.h"

#define SEQ 0
#define OMP1 1
#define OMP2 2

int find_min(int* vec, int n)
{
    int min = vec[0];
    for (int i = 1; i < n; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

int find_min_omp1(int* vec, int n)
{
    int local_mins[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
        local_mins[i] = INT_MAX;

    #pragma omp parallel
    {  
        int min = INT_MAX;
        int tid = omp_get_thread_num();
        #pragma omp for
        for (int i = 0; i < n; i++) {
            if (min >= vec[i])
                min = vec[i];
        }
        local_mins[tid] = min;
    }
    return find_min(local_mins, MAX_THREADS);
}

/* omp 3.1+ */
int find_min_omp2(int* vec, int n)
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
            case OMP1:
            res = find_min_omp1(vec, n);
            break;
            case OMP2:
            res = find_min_omp2(vec, n);
            break;
        }
        double t2 = omp_get_wtime();
        t += (t2 - t1);
    }
    free(vec);
    return t / NUM_ITER;
}


int main()
{
    struct bench_params params;
    params.start_sz = 1000000;
    params.step = 1000000;
    params.num_steps = 4;
    params.options = SEQ;
    
    bench(find_min_test, params);
    params.options = OMP1;
    bench(find_min_test, params);
    //bench(find_min_test, params);
    //bench(find_min_test, params);

    return 0;
}