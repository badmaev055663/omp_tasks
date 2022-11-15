#include "common.h"
#include "math.h"

#define SEQ 0
#define OMP 1

static double a = 0.0;
static double b = 10.0;

static double test_f(double x)
{
    return x * x * (sin(x) + cos(x) + 1);
}

double integrate(double(*f)(double), double a, double b, int n)
{
    double dx = (b - a) / n;
    double res = 0.0;
    double x = a;
    for (int i = 0; i < n; i++) {
        res += f(x) * dx;
        x += dx;
    }
    return res;
}


double integrate_omp(double(*f)(double), double a, double b, int n)
{
    double local_sums[MAX_THREADS];
    double res = 0.0;
    for (int i = 0; i < MAX_THREADS; i++)
        local_sums[i] = 0.0;

    #pragma omp parallel
    {
        double l = (b - a) / omp_get_num_threads();
        int tid = omp_get_thread_num();
        double dx = (b - a) / n;
        double x = a + tid * l;
        double sum = 0.0;
        #pragma omp for
        for (int i = 0; i < n; i++) {
            sum += f(x) * dx;
            x += dx;
        }
        local_sums[tid] = sum;
    }
    for (int i = 0; i < MAX_THREADS; i++)
        res += local_sums[i];
    return res;
}

double integrate_test(int n, int opt)
{
    double t = 0.0;

    for (int i = 0; i < NUM_ITER; i++) {
        double t1 = omp_get_wtime();
        double res;
        switch (opt) {
            case SEQ:
            res = integrate(test_f, a, b, n);
            break;
            case OMP:
            res = integrate_omp(test_f, a, b, n);
            break;
        }
        double t2 = omp_get_wtime();
        t += (t2 - t1);
    }
    return t / NUM_ITER;
}


int main(int argc, char *argv[])
{
    struct bench_params params;
    params.start_sz = 200000;
    params.step = 200000;
    params.num_steps = 10;
    
    /*strcpy(params.file_name, "out/integral_data1");
    strcpy(params.label, "integral seq");
    params.options = SEQ;
    bench(integrate_test, &params);

    strcpy(params.file_name, "out/integral_data2");
    strcpy(params.label, "integral omp");
    params.options = OMP;
    bench(integrate_test, &params);*/

    strcpy(params.file_name, "out/integral_data");
    strcpy(params.label, "integral omp");
    params.options = OMP;
    thread_bench(integrate_test, &params);
    return 0;
}