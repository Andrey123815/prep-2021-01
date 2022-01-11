#ifndef PROJECT_INCLUDE_LIST_H_
#define PROJECT_INCLUDE_LIST_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define ALLOC_ERROR -2

typedef struct Object {
    char* string;
    struct Object* next;
} Object;

typedef struct Strings_list {
    Object* head;
    Object* end;
    size_t listsize;
} Strings_list;

typedef struct Strings_list* All_strings_list;

void print_all_lists(const All_strings_list* source);

void print_list(const Strings_list* source);

void free_list(Strings_list* to_clean);

int add(Strings_list* to, const char* add_str);

#endif  // PROJECT_INCLUDE_LIST_H_
