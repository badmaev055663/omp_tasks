#include "common.h"

#define SEQ 1
#define OMP 2
#define VEC_SIZE 200


const char *file = "out/task7_data";


int create_data_file(const char* filename, int n, int m)
{
    FILE *fp;
	int *vec = (int*)malloc(sizeof(int) * n);
     
    fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("failed to create or open file: %s\n", filename);
		return -1;
	}
	for (int i = 0; i < m; i++) {
		rand_fill_vec_int(vec, n);
		for (int j = 0; j < n; j++) {
			fprintf(fp, "%d\t", vec[j]);
		}
		fprintf(fp, "\n");
	}
    fclose(fp);
	return 0;
}



int read_vectors(FILE *fp, int *vec1, int *vec2, int n)
{
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d\t", &vec1[i]);
	}
	fscanf(fp, "\n");
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d\t", &vec2[i]);
	}
	fscanf(fp, "\n");
}

long file_test_seq(FILE *fp, int n)
{
	int vec1[VEC_SIZE];
    int vec2[VEC_SIZE];
    long total = 0;
	for (int i = 0; i < n / 2; i++) {
		read_vectors(fp, vec1, vec2, VEC_SIZE);
		int res = dot_product(vec1, vec2, VEC_SIZE);
        total += res;
	}
    return total;
}

long file_test_omp(FILE *fp, int n)
{
	int vecs[4][VEC_SIZE];
    volatile int flag1 = 0;
    volatile int flag2 = 0;
    long total = 0;
    for (int i = 0; i < n / 4; i++) {
        flag1 = flag2 = 0;
        #pragma omp sections
        {
            #pragma omp section
            {
                read_vectors(fp, vecs[0], vecs[1], VEC_SIZE);
                flag1 = 1;
                read_vectors(fp, vecs[2], vecs[3], VEC_SIZE);
                flag2 = 1;
            }
            #pragma omp section
            {
check1:
                if (flag1) {
                    int res = dot_product(vecs[0], vecs[1], VEC_SIZE);
                    total += res;
check2:
                    if (flag2) {
                        res = dot_product(vecs[2], vecs[3], VEC_SIZE);
                        total += res;
                    } else {
                        goto check2;
                    }
                } else {
                    goto check1;
                }
	        }
        }
    }
    return total;
}


/* test one of dot_product versions (fixed vector size) */
double file_test(int n, int opt)
{
    double t = 0.0;
	FILE *fp;
    for (int i = 0; i < NUM_ITER; i++) {
        if (create_data_file(file, VEC_SIZE, n)) {
			return 0.0;
		}
		fp = fopen(file, "r");
        double t1 = omp_get_wtime();
		/* here n is number of vectors in file, not a vector size */
        switch (opt) {
			case SEQ:
            file_test_seq(fp, n);
            case OMP:
			file_test_omp(fp, n);
            break;
        }
        double t2 = omp_get_wtime();
        t += (t2 - t1);
		fclose(fp);
    }
    return t / NUM_ITER;
}


double file_check(int n, int opt)
{
    create_data_file(file, VEC_SIZE, n);
    FILE *fp = fopen(file, "r");
    long res1 = file_test_seq(fp, n);
    fclose(fp);
    fp = fopen(file, "r");;
	long res2 = file_test_omp(fp, n);   
	fclose(fp);
    if (res1 != res2) {
        printf("file test check failed\n");
        return -1.0;
    }
    return 0.0;
}

int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 100;
    params.step = 100;
    params.num_steps = 10;

    /*strcpy(params.file_name, "out/file_test_omp");
    strcpy(params.label, "file test omp");
    params.options = OMP;
    bench(file_test, &params);

    strcpy(params.file_name, "out/file_test_seq");
    strcpy(params.label, "file test seq");
    params.options = SEQ;
    bench(file_test, &params);*/

    strcpy(params.file_name, "out/file_test");
    strcpy(params.label, "file test");
    params.options = OMP;
    thread_bench(file_check, &params);

    return 0;
}