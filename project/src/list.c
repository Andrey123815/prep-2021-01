#include "../include/list.h"

void free_list(Strings_list* to_clean) {
    if (to_clean == NULL) {
        return;
    }

    if (to_clean->head == NULL) {
        free(to_clean);
        return;
    }

    Object* next_pointer = to_clean->head->next;

    while (next_pointer != NULL) {
        to_clean->listsize--;
        free(to_clean->head->string);
        free(to_clean->head);
        to_clean->head = next_pointer;
        next_pointer = to_clean->head->next;
    }

    free(to_clean->head->string);
    free(to_clean->head);
    to_clean->listsize--;
    to_clean->head = NULL;
    to_clean->end = NULL;
    free(to_clean);
    free(next_pointer);
}

void print_all_lists(const All_strings_list* source) {
    for (int i = 0; i < 3; ++i) {
        if (source[i]) {
            print_list(source[i]);
        } else {
            printf("%c", '|');
        }
        free_list(source[i]);
    }
}

void print_list(const Strings_list* source) {
    Object* current_object = source->head;
    if (current_object) {
        for (size_t i = 0; i < source->listsize; ++i) {
            printf("%s", current_object->string);
            current_object = current_object->next;
        }
    }
    printf("%c", '|');
}

int add(Strings_list* to, const char* add_str) {
    if (to->head == NULL) {
        to->head = malloc(sizeof(Object));

        if (!to->head) {
            return ALLOC_ERROR;
        }

        to->end = to->head;
        to->head->next = NULL;
        to->head->string = strdup(add_str);

        if (!to->head->string) {
            return ALLOC_ERROR;
        }
    } else {
        to->end->next = malloc(sizeof(Object));
        if (!to->end->next) {
            return ALLOC_ERROR;
        }

        to->end = to->end->next;
        to->end->string = strdup(add_str);
        if (!to->end->string) {
            return ALLOC_ERROR;
        }

        to->end->next = NULL;
    }

    to->listsize++;

    return 0;
}
