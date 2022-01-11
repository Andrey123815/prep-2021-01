#ifndef PROJECT_INCLUDE_TEST_H_
#define PROJECT_INCLUDE_TEST_H_

#include "bank.h"
#include "read_and_write.h"


int compare_struct(bank_client obj1, bank_client obj2);  //  Возвращает 0, если obj1 == obj2
int test_write_and_read();

#endif  //  PROJECT_INCLUDE_TEST_H_
