#include "common.h"

#define SEQ 0
#define OMP1 1
#define OMP2 2
#define OMP3 3
#define OMP4 4


void sqr_mat_mult(int **mat1, int **mat2, int **res, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			for (int k = 0; k < n; k++) {
				tmp += mat1[i][k] * mat2[k][j];
			}
			res[i][j] = tmp;
		}
	}
}


void sqr_mat_mult_omp1(int **mat1, int **mat2, int **res, int n)
{
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			for (int k = 0; k < n; k++) {
				tmp += mat1[i][k] * mat2[k][j];
			}
			res[i][j] = tmp;
		}
	}
}

void sqr_mat_mult_omp2(int **mat1, int **mat2, int **res, int n)
{
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		#pragma omp parallel for
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			for (int k = 0; k < n; k++) {
				tmp += mat1[i][k] * mat2[k][j];
			}
			res[i][j] = tmp;
		}
	}
}

void sqr_mat_mult_omp3(int **mat1, int **mat2, int **res, int n)
{
	for (int i = 0; i < n; i++) {
		#pragma omp parallel for
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			for (int k = 0; k < n; k++) {
				tmp += mat1[i][k] * mat2[k][j];
			}
			res[i][j] = tmp;
		}
	}
}

void sqr_mat_mult_omp4(int **mat1, int **mat2, int **res, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			#pragma omp parallel for reduction(+:tmp)
			for (int k = 0; k < n; k++) {
				tmp += mat1[i][k] * mat2[k][j];
			}
			res[i][j] = tmp;
		}
	}
}


double mat_mult_test(int n, int opt)
{
    double t = 0.0;
    int **mat1 = alloc_sqr_mat(n);
	int **mat2 = alloc_sqr_mat(n);
	int **res = alloc_sqr_mat(n);
	if (!mat1 || !mat2 || !res) {
		printf("failed to allocate memory\n");
		return 0.0;
	}

	for (int i = 0; i < n; i++) {
		rand_fill_vec_int(mat1[i], n);
		rand_fill_vec_int(mat2[i], n);
	}
	
    for (int i = 0; i < NUM_ITER; i++) {
		double t1 = omp_get_wtime();
		switch (opt) {
	    	case SEQ:
	    	sqr_mat_mult(mat1, mat2, res, n);
	    	break;
	    	case OMP1:
	    	sqr_mat_mult_omp1(mat1, mat2, res, n);
	    	break;
			case OMP2:
	    	sqr_mat_mult_omp2(mat1, mat2, res, n);
	    	break;
			case OMP3:
	    	sqr_mat_mult_omp3(mat1, mat2, res, n);
	    	break;
			case OMP4:
	    	sqr_mat_mult_omp4(mat1, mat2, res, n);
	    	break;
		}
		double t2 = omp_get_wtime();
		t += (t2 - t1);
	}
    free_sqr_mat(mat1, n);
	free_sqr_mat(mat2, n);
	free_sqr_mat(res, n);
    return t / NUM_ITER;
}


int matrix_cmp(int **mat1, int **mat2, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (mat1[i][j] != mat2[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}


double mat_mult_check(int n, int opt)
{
    int **mat1 = alloc_sqr_mat(n);
	int **mat2 = alloc_sqr_mat(n);
	int **res = alloc_sqr_mat(n);
	int **res1 = alloc_sqr_mat(n);
	int **res2 = alloc_sqr_mat(n);
	int **res3 = alloc_sqr_mat(n);
	int **res4 = alloc_sqr_mat(n);
	if (!mat1 || !mat2 || !res || !res1 || !res3 || !res4) {
		printf("failed to allocate memory\n");
		return 0.0;
	}
	for (int i = 0; i < n; i++) {
		rand_fill_vec_int(mat1[i], n);
		rand_fill_vec_int(mat2[i], n);
	}
	
    for (int i = 0; i < 5; i++) {	
	    sqr_mat_mult(mat1, mat2, res, n);
	    sqr_mat_mult_omp1(mat1, mat2, res1, n); 	
	    sqr_mat_mult_omp2(mat1, mat2, res2, n);
	    sqr_mat_mult_omp3(mat1, mat2, res3, n);
	    sqr_mat_mult_omp4(mat1, mat2, res4, n);
		if (matrix_cmp(res, res1, n) || matrix_cmp(res, res2, n) ||
			matrix_cmp(res, res3, n) || matrix_cmp(res, res4, n)) {
			printf("mat mult check failed\n");
			return -1.0;
		}
	}
    free_sqr_mat(mat1, n);
	free_sqr_mat(mat2, n);
	free_sqr_mat(res, n);
	free_sqr_mat(res1, n);
	free_sqr_mat(res2, n);
	free_sqr_mat(res3, n);
	free_sqr_mat(res4, n);
    return 0.0;
}

int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 50;
    params.step = 50;
    params.num_steps = 6;

    omp_set_nested(1);

    /*strcpy(params.file_name, "out/mat_mult_data1");
    strcpy(params.label, "mat mult seq");
    params.options = SEQ;
    bench(mat_mult_test, &params);

    strcpy(params.file_name, "out/mat_mult_data2");
    strcpy(params.label, "mat mult omp outer");
    params.options = OMP1;
    bench(mat_mult_test, &params);

    strcpy(params.file_name, "out/mat_mult_data3");
    strcpy(params.label, "mat mult nested");
    params.options = OMP2;
    bench(mat_mult_test, &params);

	strcpy(params.file_name, "out/mat_mult_data4");
    strcpy(params.label, "mat mult omp mid");
    params.options = OMP3;
    bench(mat_mult_test, &params);

    strcpy(params.file_name, "out/mat_mult_data5");
    strcpy(params.label, "mat mult inner");
    params.options = OMP4;
    bench(mat_mult_test, &params);*/


    /*strcpy(params.file_name, "out/mat_mult_nested");
    strcpy(params.label, "mat mult nested");
    params.options = OMP2;
    thread_bench(mat_mult_test, &params);*/

	/*strcpy(params.file_name, "out/mat_mult_mid");
    strcpy(params.label, "mat mult mid");
    params.options = OMP3;
    thread_bench(mat_mult_test, &params);*/

	strcpy(params.file_name, "out/mat_mult_outer");
    strcpy(params.label, "mat mult outer");
    params.options = OMP1;
    thread_bench(mat_mult_check, &params);

    return 0;
}