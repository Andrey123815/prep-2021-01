#include <fstream>
#include <iostream>
#include "game.h"
#include "Map.h"

enum class command_t {
    MOVE,
    KICK,
    PICK,
    THROW,
    END,
    ERROR
};

command_t read_command() {
    command_t command = command_t::ERROR;

    std::string str;
    std::cin >> str;
    if (std::cin.eof()) {
        command = command_t::END;
        return command;
    }
    if (str == "move") {
        command = command_t::MOVE;
    } else if (str == "kick") {
        std::string str_kick;
        std::cin >> str_kick;
        if (str_kick == "enemy") {
            command = command_t::KICK;
        }
    } else if (str == "pick") {
        command = command_t::PICK;
    } else if (str == "throw") {
        command = command_t::THROW;
    } else if (str.length() == 0) {
        command = command_t::END;
    }
    return command;
}

void start_game(const std::string &path) {
    std::ifstream is(path);
    Map map(is);
    is.close();

    while (!map.game_over()) {
        map.print_action();

        switch (read_command()) {
            case command_t::MOVE:
                map.player_move();
                break;
            case command_t::KICK:
                map.fight();
                break;
            case command_t::PICK:
                map.pick();
                break;
            case command_t::THROW:
                map.throw_1();
                break;
            case command_t::END:
                return;
            default:
                break;
        }
    }
}
