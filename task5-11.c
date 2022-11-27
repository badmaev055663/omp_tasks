#include "common.h"

#define SEQ 0
#define OMP1 1
#define OMP2 2
#define OMP3 3
#define CHUNK 5

// lower triangular
int** generate_triang_mat(int n)
{
    int **mat = alloc_sqr_mat(n);
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
	for (int j = 0; j <= i; j++)
	    mat[i][j] = RAND_OFFSET + rand() % MAX_RAND_VAL;
    }
    return mat;
}


int find_max_min_triang(int** mat, int n)
{
	int max = INT_MIN;
	for (int i = 0; i < n; i++) {
		int tmp = find_min(mat[i], i + 1);
		if (tmp > max)
			max = tmp;
	}
	return max;
}

int find_max_min_triang_omp1(int** mat, int n)
{
	int max = INT_MIN;
	#pragma omp parallel for schedule(static)
	for (int i = 0; i < n; i++) {
		int val = find_min(mat[i], i + 1);
		#pragma omp critical
		if (val > max)
			max = val;
	}
    return max;
}

int find_max_min_triang_omp2(int** mat, int n)
{
	int max = INT_MIN;
	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < n; i++) {
		int val = find_min(mat[i], i + 1);
		#pragma omp critical
		if (val > max)
			max = val;
	}
    return max;
}

int find_max_min_triang_omp3(int** mat, int n)
{
	int max = INT_MIN;
	#pragma omp parallel for schedule(guided)
	for (int i = 0; i < n; i++) {
		int val = find_min(mat[i], i + 1);
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
    int **mat = generate_triang_mat(n);

    for (int i = 0; i < NUM_ITER; i++) {
		double t1 = omp_get_wtime();
		int res;
		switch (opt) {
	    	case SEQ:
	    	res = find_max_min_triang(mat, n);
	    	break;
	    	case OMP1:
	    	res = find_max_min_triang_omp1(mat, n);
	    	break;
		case OMP2:
	    	res = find_max_min_triang_omp2(mat, n);
	    	break;
		case OMP3:
	    	res = find_max_min_triang_omp3(mat, n);
	    	break;
		}
		double t2 = omp_get_wtime();
		t += (t2 - t1);
	}
    free_sqr_mat(mat, n);
    return t / NUM_ITER;
}


int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 500;
    params.step = 500;
    params.num_steps = 10;

	/*params.options = SEQ;
    strcpy(params.file_name, "out/max_min_triang_data1");
    strcpy(params.label, "triang mat seq");
    bench(find_max_min_test, &params);

	params.options = OMP1;
    strcpy(params.file_name, "out/max_min_triang_data2");
    strcpy(params.label, "triang mat static");
    bench(find_max_min_test, &params);

	params.options = OMP2;
    strcpy(params.file_name, "out/max_min_triang_data3");
    strcpy(params.label, "triang mat dynamic");
    bench(find_max_min_test, &params);

	params.options = OMP3;
    strcpy(params.file_name, "out/max_min_triang_data4");
    strcpy(params.label, "triang mat guided");
    bench(find_max_min_test, &params);*/


	params.options = OMP3;
    strcpy(params.file_name, "out/max_min_triang_guided");
    strcpy(params.label, "triang mat guieded");
    thread_bench(find_max_min_test, &params);



    return 0;
}