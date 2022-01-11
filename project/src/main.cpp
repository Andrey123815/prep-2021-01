#include <string>
#include <iostream>
#include "game.h"

bool is_view_armor(false);

int main(int argc, const char** argv) {
    bool is_path = false;
    std::string param;
    std::string path;
    for (int i = 1; i < argc; ++i) {
        param = argv[i];
        if (param == "--map") {
            is_path = true;
        } else if (param == "--view-armor") {
            is_view_armor = true;
        } else {
            path = param;
        }
    }
    if (!is_path) {
        std::cout << "ERROR PATH";
        return -1;
    }
    start_game(path);
    return 0;
}
