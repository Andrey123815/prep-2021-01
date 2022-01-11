#include "../include/parser.h"

static String create_string() {
    String str = {.str = NULL, .size = 0, .capacity = 0};

    str.str = calloc(1, sizeof(char));
    if (str.str == NULL) {
        return str;
    }

    str.capacity = 1;
    return str;
}

static int resize(String *return_string) {
    if (return_string == NULL) {
        return ALLOC_ERROR;
    }

    char *tmp = realloc(return_string->str, return_string->capacity * 2);

    if (tmp == NULL) {
        return ALLOC_ERROR;
    }

    return_string->str = tmp;
    return_string->capacity *= 2;

    return 0;
}

static void free_string(String *string) {
    if (string->str == NULL) {
        return;
    }

    free(string->str);

    string->str = NULL;
    string->size = 0;
    string->capacity = 0;
}

static char* add_to_string(String* return_string, char letter_to_add) {
    if (return_string == NULL) {
        return NULL;
    }
    if (return_string->capacity <= return_string->size + 1) {
        if (resize(return_string) != 0) {
            return NULL;
        }
    }
    return_string->str[return_string->size++] = letter_to_add;
    return_string->str[return_string->size] = '\0';

    return return_string->str;
}

// check if letter is service (does not belong to english + !,.- etc.
static bool letter_is_service(int letter) {
    if ((letter >= 33 && letter <= 125) || (letter < 0)) {
        return false;
    }
    return true;
}


static int check_second_symbol(FILE* source, int letter, const char** compare_to,
                               const unsigned int* compare_to_length) {
    unsigned int j = 2;

    for (unsigned int i = 0; i < 4; ++i) {
        if (letter == compare_to[i][1]) {
            while (j <= compare_to_length[i] - 1) {
                letter = fgetc(source);
                letter = toupper(letter);

                if (letter != compare_to[i][j]) {
                    break;
                }

                j++;
            }

            if (j == compare_to_length[i]) {
                return (int)i;
            }
        }
    }

    return -1;
}

static int check_other_conditions(FILE* source, int letter, const char** compare_to,
                                  const unsigned int* compare_to_length) {
    while (letter != -1) {
        letter = toupper(letter);

        for (unsigned int i = 0; i < 4; ++i) {
            if (letter == compare_to[i][0]) {
                unsigned int j = 1;

                while (j <= compare_to_length[i] - 1) {
                    int prev_letter = letter;
                    letter = fgetc(source);
                    letter = toupper(letter);

                    if (letter != compare_to[i][j]) {
                        if (prev_letter == '\n' && letter == '\n') {
                            ungetc(letter, source);
                            ungetc(prev_letter, source);
                            return -1;
                        }
                        if (prev_letter == '\n' && i < 3) {
                            ungetc(letter, source);
                            letter = '\n';
                        }
                        break;
                    }

                    j++;
                }

                if (j == compare_to_length[i]) {
                    return (int)i;
                }
            }
        }
        letter = fgetc(source);
    }
    return -1;
}

// searches file for given string. not sensitive to register
static int tag_compare(FILE* source, const char** compare_to, unsigned int *compare_length) {
    int letter = fgetc(source);
    int status = check_second_symbol(source, letter, compare_to, compare_length);
    if (status != -1) {
        return status;
    }

    return check_other_conditions(source, letter, compare_to, compare_length);
}

// searches file for given string. sensitive to register
static int key_compare(FILE* source, const char* boundary) {
    size_t length_compare_to = strlen(boundary);

    int letter = fgetc(source);

    while (letter != EOF) {
        if (letter == boundary[0]) {
            unsigned long i = 1;

            while (i <= length_compare_to - 1) {
                letter = fgetc(source);

                while (letter == ' ') {
                    letter = fgetc(source);
                }

                if (letter != boundary[i]) {
                    break;
                }

                i++;
            }

            if (i == length_compare_to) {
                letter = fgetc(source);
                if (letter == '-') {
                    letter = fgetc(source);
                    if (letter == '-') {
                        return 0;
                    }
                }
                return 1;
            }
        }
        if (letter == -1) {
            return -1;
        }
        letter = fgetc(source);
    }
    return -1;
}

// checks if the string ends in : (does not count spaces)
static bool ends_in_ddot(const char* string) {
    size_t string_length = strlen(string);
    if (string_length != 0) {
        if (string_length != 1) {
            size_t i = string_length - 1;

            while (i != 0 && string[i] != ':') {
                i--;
            }

            if (i == 0 && string[i] != ':') {
                return false;
            }
            if (i == string_length - 1) {
                return true;
            }

            if (i < string_length - 1) {
                if (letter_is_service(string[i + 1])) {
                    return true;
                }
            }
        }
        if (string[0] == ':') {
            return true;
        }
    }
    return false;
}

// reads info from tag (requires position after needed tag)
static int read_until_string_end(FILE* source_message, Strings_list* list) {
    bool flag_not_ends_in_ddot = true;
    char read_letter = fgetc(source_message);

    while (letter_is_service(read_letter)) {
        read_letter = fgetc(source_message);
    }

    do {
        String buffer_string = create_string();
        if (buffer_string.str == NULL) {
            free_string(&buffer_string);
            return ALLOC_ERROR;
        }

        while (!letter_is_service(read_letter)) {
            add_to_string(&buffer_string, (char)read_letter);
            read_letter = fgetc(source_message);
        }

        int to_unget_key = 0;
        while (read_letter == ' ') {
            add_to_string(&buffer_string, read_letter);
            read_letter = fgetc(source_message);
            to_unget_key = 1;
        }

        if (buffer_string.size != 0) {
            add(list, buffer_string.str);
        }

        flag_not_ends_in_ddot = !ends_in_ddot(buffer_string.str);

        if (flag_not_ends_in_ddot == false) {
            int letter_char_1;
            letter_char_1 = fgetc(source_message);
            if (letter_char_1 == ' ') {
                flag_not_ends_in_ddot = true;
            }
        }

        char previous_letter = ' ';
        if (to_unget_key == 1) {
            ungetc(read_letter, source_message);
        } else {
            previous_letter = read_letter;
        }

        read_letter = fgetc(source_message);
        free_string(&buffer_string);

        if (previous_letter == read_letter && previous_letter == '\n') {
            ungetc(read_letter, source_message);
            ungetc(previous_letter, source_message);
            return 0;
        }

        if (previous_letter != read_letter && previous_letter == '\n' &&
        read_letter != ' ' && read_letter != '\t') {
            ungetc(read_letter, source_message);
            return 0;
        }

        if (previous_letter != read_letter && ((previous_letter == '\r'
                                && read_letter == '\n'))) {
            read_letter = fgetc(source_message);
            if (read_letter == ' ' || read_letter == '\t') {
                while (read_letter == ' ' || read_letter == '\t') {
                    read_letter = fgetc(source_message);
                }
                add(list, " ");
            } else {
                ungetc(read_letter, source_message);
                return 0;
            }
        }
    } while (flag_not_ends_in_ddot);

    return 0;
}

// sets position to read tag (additional check if tag is in start of file
int read_tags(FILE* source_message, Strings_list* list, const char** compare_to,
              unsigned int* compare_to_length, String* boundary, int* parts) {
    int status = tag_compare(source_message, compare_to, compare_to_length);
    if (status != -1) {
        if (status == CONTENT_TYPE) {
            if (search_current_tag(source_message, "MULTIPART")) {
                if (search_current_tag(source_message, "BOUNDARY=")) {
                    get_boundary(source_message, boundary);
                } else {
                    *parts = 1;
                }
            } else {
                *parts = 1;
            }
            return status;
        }
        read_until_string_end(source_message, list);
    }
    return status;
}

int search_current_tag(FILE* source, const char* compare_to) {
    size_t length_compare_to = strlen(compare_to);

    int prev_letter = ' ';
    int letter = fgetc(source);

    while (letter != EOF && letter != ':') {
        letter = toupper(letter);
        if (letter == compare_to[0] && !(prev_letter >= 'A' && prev_letter <= 'Z')) {
            unsigned long i;

            for (i = 1; i <= length_compare_to - 1; i++) {
                letter = fgetc(source);
                letter = toupper(letter);

                if (letter != compare_to[i]) {
                    break;
                }
            }

            if (i == length_compare_to) {
                return true;
            }
        }
        prev_letter = letter;
        letter = fgetc(source);
        if (prev_letter == '\n' && letter != ' ' && letter != '\t') {
            ungetc(letter, source);
            return false;
        }
    }
    return false;
}

String* get_boundary(FILE* source_message, String* key) {
    if (key->str == NULL) {
        return NULL;
    }
    String initial_key = create_string();
    if (initial_key.str == NULL) {
        free_string(&initial_key);
        free_string(key);
        return NULL;
    }
    int letter = fgetc(source_message);

    int counter = 0;
    while (letter_is_service(letter) || (letter == '\"' && counter == 0)) {
        if (letter == '\"') {
            counter++;
        }
        letter = fgetc(source_message);
    }

    if (counter == 0) {
        while (letter != '\n' && letter != ' ' && letter != '\r') {
            add_to_string(&initial_key, (char) letter);
            letter = fgetc(source_message);
        }
    } else {
        while (letter != '\"') {
            add_to_string(&initial_key, (char) letter);
            letter = fgetc(source_message);
        }
    }

    if (initial_key.str[0] == '\"') {
        memmove(initial_key.str, initial_key.str + 1, initial_key.size - 1);
    }
    add_to_string(key, '-');
    add_to_string(key, '-');
    unsigned int i = 0;
    while (i < initial_key.size) {
        add_to_string(key, initial_key.str[i]);
        i++;
    }

    if (letter == '\n') {
        ungetc(letter, source_message);
    }
    free_string(&initial_key);

    return key;
}

static void find_double_enter(FILE* source_message) {
    int double_enter = 0;

    while (double_enter != 2) {
        int letter = fgetc(source_message);
        if (letter != '\r' && letter != '\n') {
            double_enter = 0;
        }
        while (letter != '\n') {
            letter = fgetc(source_message);
        }
        double_enter++;
    }
}

// counts number and checks existence of mail body
int read_body(FILE* source_message, String* key) {
    find_double_enter(source_message);

    int letter = fgetc(source_message);

    while (letter == '\n' || letter == '\r') {
        letter = fgetc(source_message);
    }

    ungetc(letter, source_message);

    int parts_count = 0;
    int status = key_compare(source_message, key->str);

    while (status != EOF) {
        parts_count += status;
        status = key_compare(source_message, key->str);
    }

    return parts_count;
}

// init/read/free
int start_list_and_parse_tag(FILE* source_message, Strings_list* strings, const char** compare_to,
                             unsigned int* compare_to_length, String* boundary, int* parts) {
    if (!strings) {
        free_string(boundary);
        return ALLOC_ERROR;
    }
    strings->head = NULL;
    strings->listsize = 0;
    strings->end = NULL;

    return read_tags(source_message, strings, compare_to, compare_to_length, boundary, parts);
}

int parse_letter(const char* path_to_eml) {
    FILE* message = fopen(path_to_eml, "r");

    if (!message) {
        return ALLOC_ERROR;
    }

    All_strings_list main_list[3] = {0};  // massive_of_lists
    const char* lexems[4] = {SCAN_SENDER_TAG, SCAN_ADDRESS_TAG,
                             SCAN_DATE_TAG, SCAN_CONTENT_TYPE};
    unsigned int lexems_length[4] = {SENDER_TAG_LENGTH, ADDRESS_TAG_LENGTH,
                                     DATE_TAG_LENGTH, CONTENT_TYPE_TAG_LENGTH};

    String boundary = create_string();
    int parts = 0;
    int status_order[4] = {0};
    int status_content_type = 0;

    for (int i = 0; i < 4; ++i) {
        Strings_list* strings = calloc(1, sizeof(Strings_list));
        status_order[i] =  start_list_and_parse_tag(message, strings,
                                                    lexems, lexems_length, &boundary, &parts);

        if (status_order[i] == BEGIN_BODY) {
            free_list(strings);
            break;
        }

        if (status_order[i] != CONTENT_TYPE) {
            main_list[status_order[i]] = strings;
        } else {
            status_content_type = 1;
            free_list(strings);
        }
    }

    print_all_lists(main_list);

    if (status_content_type == 1 && parts != 1) {
        printf("%d", read_body(message, &boundary));
    } else {
        printf("1");
    }

    free_string(&boundary);

    fclose(message);
    return 0;
}
