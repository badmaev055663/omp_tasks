#include "common.h"

#define SEQ 0
#define OMP 1


int find_max_min(int** mat, int n)
{
	int max = INT_MIN;
	for (int i = 0; i < n; i++) {
		int tmp = find_min(mat[i], n);
		if (tmp > max)
			max = tmp;
	}
	return max;
}

int find_max_min_omp(int** mat, int n)
{
	int max = INT_MIN;
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		int val = find_min(mat[i], n);
		#pragma omp critical
		if (val > max)
			max = val;
	}
    return max;
}

/* test one of find_max_min versions (fixed matrix size) */
double find_max_min_test(int n, int opt)
{
    double t = 0.0;
    int **mat = alloc_sqr_mat(n);

	for (int i = 0; i < n; i++)
		rand_fill_vec_int(mat[i], n);
	
    for (int i = 0; i < NUM_ITER; i++) {
		double t1 = omp_get_wtime();
		int res;
		switch (opt) {
	    	case SEQ:
	    	res = find_max_min(mat, n);
	    	break;
	    	case OMP:
	    	res = find_max_min_omp(mat, n);
	    	break;
		}
		double t2 = omp_get_wtime();
		t += (t2 - t1);
	}
    free_sqr_mat(mat, n);
    return t / NUM_ITER;
}

double find_max_min_check(int n, int opt)
{
    int **mat = alloc_sqr_mat(n);

	for (int i = 0; i < n; i++)
		rand_fill_vec_int(mat[i], n);
	
    for (int i = 0; i < 5; i++) {
		int res1 = find_max_min(mat, n);    	
	    int res2 = find_max_min_omp(mat, n);
		if (res1 != res2) {
            printf("find max min check failed\n");
            free_sqr_mat(mat, n);
            return -1.0;
        }
	}
    free_sqr_mat(mat, n);
    return 0.0;
}

int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 500;
    params.step = 500;
    params.num_steps = 8;

    /*params.options = SEQ;
    strcpy(params.file_name, "out/max_min_data1");
    strcpy(params.label, "max min seq");
    bench(find_max_min_test, &params);

    params.options = OMP;
    strcpy(params.file_name, "out/max_min_data2");
    strcpy(params.label, "max min omp");
    bench(find_max_min_test, &params);*/
	params.options = OMP;
    strcpy(params.file_name, "out/max_min_data");
    strcpy(params.label, "max min omp");
    //thread_bench(find_max_min_test, &params);
	thread_bench(find_max_min_check, &params);
	
    return 0;
}