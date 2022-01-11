#ifndef PROJECT_INCLUDE_FUNC_H_
#define PROJECT_INCLUDE_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "read_and_write.h"

void create_record(const char *record);
void create_transaction(const char *file_transaction);
void update_record(const char *record, const char *transaction, const char *blackrecord);

#endif  //  PROJECT_INCLUDE_FUNC_H_
