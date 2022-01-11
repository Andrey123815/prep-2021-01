#ifndef PROJECT_INCLUDE_PARSER_H_
#define PROJECT_INCLUDE_PARSER_H_
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "list.h"

#define SCAN_SENDER_TAG "\nFROM:"
#define SENDER_TAG_LENGTH 6
#define SCAN_ADDRESS_TAG "\nTO:"
#define ADDRESS_TAG_LENGTH 4
#define SCAN_DATE_TAG "\nDATE:"
#define DATE_TAG_LENGTH 6
#define SCAN_CONTENT_TYPE "\nCONTENT-TYPE:"
#define CONTENT_TYPE_TAG_LENGTH 14

#define ALLOC_ERROR -2

#define FROM 0
#define TO 1
#define DATE 2
#define CONTENT_TYPE 3
#define BEGIN_BODY -1

typedef struct String {
    char* str;
    size_t size;
    size_t capacity;
} String;

int start_list_and_parse_tag(FILE* source_message, Strings_list* strings, const char** compare_to,
                             unsigned int* compare_to_length, String* boundary, int* parts);
int search_current_tag(FILE* source, const char* compare_to);
String* get_boundary(FILE* source_message, String* boundary);
int read_tags(FILE* source_message, Strings_list* list, const char** compare_to,
              unsigned int* compare_to_length, String* boundary, int* parts);
int read_body(FILE* source_message, String* boundary);
int parse_letter(const char* path_to_eml);

#endif  // PROJECT_INCLUDE_PARSER_H_
