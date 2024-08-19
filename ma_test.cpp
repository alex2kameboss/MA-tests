#include "ma_test_helper.h"
#include "ImtMatrixAccelerator.h"
#include "MyUnitTestFramework.h"

#include <cstdint>
#include <iostream>
#include <iomanip>

template <typename T, typename Q>
void debug(T* a, T* b, Q* res_sw, Q* res_hw) {
#ifdef DEBUG
    printf("\na = \n");
    print(a, N, M);
    printf("b = \n");
    print(b, P, N);
    printf("sw = \n");
    print(res_sw, P, M);
    printf("hw = \n");
    print(res_hw, P, M);
#endif
}

#define TEST_BASE(DTYPE_I, DTYPE_O, ACC, REF) \
    DTYPE_I a[M * N], b[N * P]; \
    DTYPE_O res_sw[M * P], res_hw[M * P]; \
    init(a, N, M); \
    init(b, P, N); \
    MA_DEFINE_##DTYPE_I(0, N, M); \
    MA_DEFINE_##DTYPE_I(1, P, N); \
    MA_DEFINE_##DTYPE_O(2, P, M); \
    MA_LOAD_REGISTER(0, a); \
    MA_LOAD_REGISTER(1, b); \
    ACC(2, 0, 1); \
    MA_STORE_REGISTER(2, res_hw); \
    REF(a, b, res_sw, M, N, P); \
    debug(a, b, res_sw, res_hw); \
    return cmp(res_hw, res_sw, M * P); \

#define ADD_TEST_BASE(DTYPE_I, DTYPE_O) TEST_BASE(DTYPE_I, DTYPE_O, MA_VV_ADD, add)
#define SUB_TEST_BASE(DTYPE_I, DTYPE_O) TEST_BASE(DTYPE_I, DTYPE_O, MA_VV_SUB, sub)
#define DIV_TEST_BASE(DTYPE_I, DTYPE_O) TEST_BASE(DTYPE_I, DTYPE_O, MA_VV_DIV, div)
#define MULT_TEST_BASE(DTYPE_I, DTYPE_O) TEST_BASE(DTYPE_I, DTYPE_O, MA_VV_MULT, mult)
#define SMULT_TEST_BASE(DTYPE_I, DTYPE_O) TEST_BASE(DTYPE_I, DTYPE_O, MA_VV_SMULT, smult)

#define ADD_TEST(DTYPE) bool add_test_##DTYPE() { ADD_TEST_BASE(DTYPE, DTYPE) }
#define SUB_TEST(DTYPE) bool sub_test_##DTYPE() { SUB_TEST_BASE(DTYPE, DTYPE) }
#define DIV_TEST(DTYPE) bool div_test_##DTYPE() { DIV_TEST_BASE(DTYPE, DTYPE) }
#define MULT_TEST(DTYPE) bool mult_test_##DTYPE() { MULT_TEST_BASE(DTYPE, DTYPE) }
#define SMULT_TEST(DTYPE) bool smult_test_##DTYPE() { SMULT_TEST_BASE(DTYPE, DTYPE) }

#define GROUP_TEST(DTYPE) \
    ADD_TEST(DTYPE) \
    SUB_TEST(DTYPE) \
    DIV_TEST(DTYPE) \
    MULT_TEST(DTYPE) \
    SMULT_TEST(DTYPE) 

GROUP_TEST(int8_t);
GROUP_TEST(uint8_t);
GROUP_TEST(int16_t);
GROUP_TEST(uint16_t);
GROUP_TEST(int32_t);
GROUP_TEST(uint32_t);

#define ADD_DTYPE_TESTS(DTYPE) \
    MY_TEST(Addition, DTYPE, add_test_##DTYPE) \
    MY_TEST(Substraction, DTYPE, sub_test_##DTYPE) \
    MY_TEST(Division, DTYPE, div_test_##DTYPE) \
    MY_TEST(Multiplication, DTYPE, mult_test_##DTYPE) \
    MY_TEST(SMultiplication, DTYPE, smult_test_##DTYPE) \

int main() {
    MY_INIT

    ADD_DTYPE_TESTS(uint8_t);
    ADD_DTYPE_TESTS(int8_t);
    ADD_DTYPE_TESTS(uint16_t);
    ADD_DTYPE_TESTS(int16_t);
    ADD_DTYPE_TESTS(uint32_t);
    ADD_DTYPE_TESTS(int32_t);

    int succed = 0, failed = 0, all = 0;
    for ( const auto& testGroup: tests ) {
        std::cout << testGroup.first << std::endl;
        for ( const auto& test: testGroup.second ) {
            all++;
            std::cout << " " << std::setw(10) << test.first << " ";
            if ( !test.second() )
                failed++, std::cout << "FAILED" << std::endl;
            else
                succed++, std::cout << "SUCCED" << std::endl;
        }
    }

    std::cout << "Success rate [%]: " << (float) succed / all * 100 << std::endl;
    std::cout << "Fail rate [%]: " << (float) failed / all * 100 << std::endl;
}