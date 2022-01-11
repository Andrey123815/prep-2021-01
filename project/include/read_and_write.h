#ifndef PROJECT_INCLUDE_READ_AND_WRITE_H_
#define PROJECT_INCLUDE_READ_AND_WRITE_H_

#include <stdio.h>
#include <stdbool.h>
#include "bank.h"

int read_client(const char *filename, bank_client *data, int num_client, bool is_stdin);
int read_transfer(const char *filename, bank_client *data, int num_transfer, bool is_stdin);
int write_to_file(const char *filename, bank_client *data);
int read_data(FILE * fptr, bank_client *data);

#endif  //  PROJECT_INCLUDE_READ_AND_WRITE_H_
