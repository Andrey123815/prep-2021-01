#ifndef PROJECT_INCLUDE_MAP_H_
#define PROJECT_INCLUDE_MAP_H_

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Clothes.h"

class Map {
 public:
    Map();
    explicit Map(std::istream &is);
    ~Map() = default;

    Enemy name_def_en(std::string name);
    Clothes name_def_cl(std::string name);
    void check_player_clothes();
    void fight();
    int player_move();
    void print_action();
    void pick();
    void throw_1();
    bool game_over();

 private:
    void check_enemies(int yp, int xp);
    std::string get_enemy_name(int number);
    std::string get_armor_name(int number);

    int rows;
    int cols;
    int x;
    int y;
    std::vector<std::vector<int>> map;
    std::vector<std::string> massive_enemy;
    std::vector<std::string> massive_clothes;
    std::vector<std::pair<std::string, int>> player_clothes;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Clothes> clothes;
};

#endif  // PROJECT_INCLUDE_MAP_H_
