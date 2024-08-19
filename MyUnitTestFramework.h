#ifndef MY_UNIT_TEST_FRAMEWORK_H
#define MY_UNIT_TEST_FRAMEWORK_H

#include <map>

typedef bool (*TestFunction)(void);

#define _STR(X) #X

#define MY_INIT std::map<std::string, std::map<std::string, TestFunction>> tests;

#define MY_TEST(GROUP, TEST, FN) tests[_STR(GROUP)][_STR(TEST)] = &FN;

#endif