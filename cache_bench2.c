#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define RUNS 30
#define TARGET_ELEMS (1u << 24)

static volatile long long sink;

static double now_ns(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

static long long sum_row(const int *a, int n) {
    long long s = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            s += a[(size_t)i * n + j];
    return s;
}

static long long sum_col(const int *a, int n) {
    long long s = 0;
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            s += a[(size_t)i * n + j];
    return s;
}

int main(void) {
#ifdef _WIN32
    SetThreadAffinityMask(GetCurrentThread(), 1);
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif
    printf("N,array_KB,order,run,ns_per_element\n");
    static const int sizes[] = {16,32,64,100,128,200,256,300,500,512,700,1000,1024,1500,2000,2048,3000,4000,4096};
    for (int si = 0; si < (int)(sizeof sizes / sizeof sizes[0]); si++) {
        int n = sizes[si];
        size_t elems = (size_t)n * n;
        int *a = malloc(elems * sizeof(int));
        if (!a) { fprintf(stderr, "malloc failed\n"); return 1; }
        for (size_t k = 0; k < elems; k++) a[k] = (int)(k % 100);

        unsigned reps = TARGET_ELEMS / elems;
        if (reps < 1) reps = 1;

        for (int order = 0; order < 2; order++) {
            sink = order ? sum_col(a, n) : sum_row(a, n);
            for (int r = 0; r < RUNS; r++) {
                long long s = 0;
                double t0 = now_ns();
                for (unsigned k = 0; k < reps; k++)
                    s += order ? sum_col(a, n) : sum_row(a, n);
                double t1 = now_ns();
                sink = s;
                printf("%d,%zu,%s,%d,%.4f\n", n, elems * sizeof(int) / 1024,
                       order ? "column" : "row", r,
                       (t1 - t0) / ((double)reps * elems));
            }
        }
        free(a);
    }
    return 0;
}