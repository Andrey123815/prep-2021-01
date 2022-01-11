#include "../include/parser.h"

int main(int argc, const char** argv) {
    if (argc != 2) {
        return -1;
    }

    const char* path_to_eml = argv[1];

    return parse_letter(path_to_eml);
}
