#include <stdio.h>
#include <stdlib.h>
#include "ImtMatrixAccelerator.h"

#define M 2
#define N 3
#define P 2
#define RND_FACTOR 10

void init(int *ptr, int w, int h) {
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            ptr[i * w + j] = rand() % RND_FACTOR;
}

void mult(int *a, int *b, int *c, int m, int n, int p) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            c[i * p + j] = 0;
            for (int k = 0; k < n; ++k) 
                c[i * p + j] += a[i * n + k] * b[k * p + j];
        }
    }
}

void print(int *ptr, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            printf("%d ", ptr[i * w + j]);
        printf("\n");
    }

}

int main() {
    int m = M, n = N, p = P;

    int a[M * N], b[N * P], res_sw[M * P], res_hw[M * P];

    init(a, N, M);
    init(b, P, N);

    //printf("Define SR0\n");
    /*asm volatile
    (
        "v2ddef32 x0, %[x], %[y]\n\t"
        : 
        : [x] "r" (n), [y] "r" (m)
    );*/
    MA_DEFINE_int32_t(0, n, m);
    MA_DEFINE_int32_t(1, p, n);
    MA_DEFINE_int32_t(2, p, m);
    //printf("Define SR1\n");
    //asm volatile
    /*(
        "v2ddef32 x1, %[x], %[y]\n\t"
        : 
        : [x] "r" (p), [y] "r" (n)
    );
    printf("Define SR2\n");
    asm volatile
    (
        "v2ddef32 x2, %[x], %[y]\n\t"
        : 
        : [x] "r" (p), [y] "r" (m)
    );*/

    /*printf("Load SR0\n");
    asm volatile
    (
        "v2dld x0, %[x]\n\t"
        :
        : [x] "m" (a)
    );
    printf("Store SR1\n");
    asm volatile
    (
        "v2dld x1, %[x]\n\t"
        :
        : [x] "m" (b)
    );*/

    MA_LOAD_REGISTER(0, a);
    MA_LOAD_REGISTER(1, b);

    mult(a, b, res_sw, M, N, P);

    printf("a = \n");
    print(a, N, M);

    printf("b = \n");
    print(b, P, N);

    printf("sw = \n");
    print(res_sw, P, M);

    printf("Mult\n");
    //asm volatile( "v2dmul.vv x2, x0, x1" );
    MA_VV_MULT(2, 0, 1);
    printf("Store SR2\n");
    /*asm volatile
    (
        "v2dst x2, %[x]\n\t"
        :
        : [x] "m" (res_hw)
    );*/
    MA_STORE_REGISTER(2, res_hw);

    printf("hw = \n");
    print(res_hw, P, M);

    for ( int i = 0; i < M * P; ++i ) {
        if ( res_sw[i] != res_hw[i] ) {
            printf("FAILED\n");
            return 1;
        }
    }

    printf("PASSED\n");

    return 0;
}