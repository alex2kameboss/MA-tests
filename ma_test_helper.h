#ifndef MA_TEST_HELPER_H
#define MA_TEST_HELPER_H

#ifndef RND_FACTOR
#define RND_FACTOR 10
#endif

#ifndef M
#define M 6
#endif

#ifndef N
#define N 6
#endif

#ifndef P
#define P 6
#endif

#ifndef C
#define C 2
#endif

#include <cstdlib>
#include <cstdio>

template <typename T>
void init(T *ptr, int w, int h) {
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            ptr[i * w + j] = rand() % RND_FACTOR + 1;
}

template <typename T>
bool cmp(T* src1, T* src2, int len) {
    for ( int i =0 ; i < len; ++i )
        if (src1[i] != src2[i])
            return false;
    return true;
}

template <typename T, typename Q>
void mult(T *a, T *b, Q *c, int m, int n, int p) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            c[i * p + j] = 0;
            for (int k = 0; k < n; ++k) 
                c[i * p + j] += a[i * n + k] * b[k * p + j];
        }
    }
}

#define SCALAR_OPERATION_FN(NAME, OP) \
template <typename T, typename Q> \
void NAME(T *a, T *b, Q *c, int m, int n, int p) { \
    for (int i = 0; i < m; ++i) { \
        for (int j = 0; j < n; ++j) { \
            c[i * n + j] = a[i * n + j] OP b[i * n + j]; \
        } \
    } \
} 

SCALAR_OPERATION_FN(add, +);
SCALAR_OPERATION_FN(sub, -);
SCALAR_OPERATION_FN(div, /);
SCALAR_OPERATION_FN(smult, *);

template <typename T, typename Q>
void cnv(T* a, T* b, Q* c, int m_a, int n_a, int m_b, int n_b) {
    int m_c = m_a - m_b + 1;
    int n_c = n_a - n_b + 1;
    for ( int i = 0; i < m_c; ++i ) {
        for ( int j = 0; j < n_c; ++j ) {
            c[ i * n_c + j ] = 0;
            for ( int ii = 0; ii < m_b; ++ii ) 
                for ( int jj = 0; jj < n_b; ++jj )
                    c[ i * n_c + j ] += a[ (i + ii) * m_a + j + jj ] * b[ ii * m_b + jj ];
        }
    }
}

template <typename T>
void print(T *ptr, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            printf("%d ", ptr[i * w + j]);
        printf("\n");
    }
}

#endif