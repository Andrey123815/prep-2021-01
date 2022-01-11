#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Map.h"
#include "Clothes.h"

extern bool is_view_armor;

Enemy Map::name_def_en(std::string name) {
    if (name == "wolf") {
        Wolf wolf;
        return wolf;
    }
    if (name == "dog") {
        Dog dog;
        return dog;
    }
    if (name == "rat") {
        Rat rat;
        return rat;
    }
    Enemy enemy1;
    return enemy1;
}

Clothes Map::name_def_cl(std::string name) {
    if (name == "armor") {
        Armor armor;
        return armor;
    }
    if (name == "helmet") {
        Helmet helmet;
        return helmet;
    }
    if (name == "shield") {
        Shield shield;
        return shield;
    }
    if (name == "pants") {
        Pants pants;
        return pants;
    }
    if (name == "T-Shirt") {
        TShirt tshirt;
        return tshirt;
    }
    Clothes clothes11;
    return clothes11;
}

Map::Map() : rows(0), cols(0), player() {}

Map::Map(std::istream &is) : player()  {
    is >> cols >> rows;
    if (rows < 1 || cols < 1) {
        std::cout << "error creation map\n";
        std::cout << "rows cols: " << this->rows << " " << this->cols << "\n";
    }
    map.resize(rows, std::vector<int>(cols));
    int k_en = 1;
    int k_cl = -1;
    while (!is.eof()) {
        int x_enemy;
        int y_enemy;
        std::string name;
        if (!(is >> y_enemy>> x_enemy  >> name)) {
            break;
        }
        if (name == "wolf" || name == "rat" || name == "dog") {
            Enemy enemy_stream = name_def_en(name);
            enemies.push_back(enemy_stream);
            massive_enemy.push_back(name);
            map[x_enemy][y_enemy] = k_en;
            k_en++;
        } else {
            Clothes clothes_stream = name_def_cl(name);
            clothes.push_back(clothes_stream);
            massive_clothes.push_back(name);
            map[x_enemy][y_enemy] = k_cl;
            k_cl--;
        }
    }
}

int Map::player_move() {
    std::string str;
    std::cin >> str;
    int xp = player.get_x();
    int yp = player.get_y();
    if (str == "left" || str == "z") {
        player.moving_player(0, -1);
        check_enemies(yp + 0, xp - 1);
        map[yp][xp] = 0;
        return player.get_y();
    }
    if (str == "right" || str == "w") {
        player.moving_player(0, 1);
        check_enemies(yp + 0, xp + 1);
        map[yp][xp] = 0;
        return player.get_y();
    }
    if (str == "down" || str == "a") {
        player.moving_player(-1, 0);
        check_enemies(yp - 1, xp + 0);
        map[yp][xp] = 0;
        return player.get_x();
    }
    if (str == "up" || str == "d") {
        player.moving_player(1, 0);
        check_enemies(yp + 1, xp + 0);
        map[yp][xp] = 0;
        return player.get_x();
    }
    return 0;
}

std::string Map::get_enemy_name(int number) {
    return massive_enemy[number-1];
}

std::string Map::get_armor_name(int number) {
    return massive_clothes[number - 1];
}

void Map::check_enemies(int yp, int xp) {
    int ident = abs(map[yp][xp]);
    if (map[yp][xp] == 0) {
        std::cout << "\nmoved" << std::endl;
    } else if (map[yp][xp] > 0) {
        std::cout << "\n" << get_enemy_name(ident) << " found, " << enemies[ident-1].get_value_hp() << " hp" << "\n";
    } else {
        std::cout << "\n" << get_armor_name(ident) << " found\n";
    }
}

void Map::print_action() {
    int xp = player.get_x();
    int yp = player.get_y();
    std::cout << "Supported actions:\n";
    if (map[yp][xp] > 0) {
        std::cout << " * kick enemy\n";
    } else {
        if (xp > 0) {
            std::cout << " * move left\n";
        }
        if (xp < this->cols - 1) {
            std::cout << " * move right\n";
        }
        if (yp > 0) {
            std::cout << " * move down\n";
        }
        if (yp < this->rows - 1) {
            std::cout << " * move up\n";
        }
        if (map[yp][xp] < 0) {
            int k = 0;
            std::string name_input = massive_clothes[abs(map[yp][xp])-1];
            for (unsigned int i = 0; i < player_clothes.size(); ++i) {
                if (name_input == player_clothes[i].first) {
                    k++;
                    break;
                }
            }
            if (k == 0) {
                std::cout << " * pick " << massive_clothes[abs(map[yp][xp])-1] << "\n";
            }
            check_player_clothes();
        }
        if (rows == 1 && cols == 1) {
            std::cout << "\n";
        }
    }
    std::cout << xp << " x " << yp << ", hp: " << player.get_value_hp();
    if (is_view_armor) {
        std::cout << ", armor: " << player.get_arm() << " > ";
    } else {
        std::cout << " > ";
    }
}

void Map::check_player_clothes() {
    if (player_clothes.size() != 0) {
        for (unsigned int i = 0; i < player_clothes.size(); ++i) {
            std::cout << " * throw " << player_clothes[i].first << "\n";
        }
    }
}

void Map::fight() {
    int xx = player.get_x();
    int yy = player.get_y();
    int ident = map[yy][xx];
    enemies[ident-1].kick(player.get_value_dmg());
    if (!enemies[ident-1].is_alive()) {
        map[yy][xx] = 0;
        std::cout << "\nenemy killed\n";
        return;
    }

    if (player.get_arm() >= enemies[ident-1].get_value_dmg()) {
        player.kick(1);
    } else {
        player.kick(enemies[ident-1].get_value_dmg() - player.get_arm());
    }

    if (!player.is_alive()) {
        std::cout << "\nplayer died\n";
        return;
    }
    std::cout << "\nenemy kicked. Enemy hp: " << enemies[ident-1].get_value_hp() << '\n';
}

bool Map::game_over() {
    return !player.is_alive();
}

bool compare(const std::pair<std::string, int> &Elem1, const std::pair<std::string, int> &Elem2) {
    return Elem1.first < Elem2.first;
}

void Map::pick() {
    std::string str;
    std::cin >> str;
    if (!std::cin.fail()) {
        int wgt_max = 20;
        int xx = player.get_x();
        int yy = player.get_y();
        int ident = abs(map[yy][xx]);
        std::string armor_name = massive_clothes[ident - 1];
        int wgt_gained = player.wgt_gain(clothes[ident - 1].get_wgt());
        if (wgt_gained < wgt_max) {
            player_clothes.push_back(std::make_pair(armor_name, ident-1));
            player.arm_gain(clothes[ident - 1].get_arm());
            map[yy][xx] = 0;
            sort(player_clothes.begin(), player_clothes.end(), compare);
            std::cout << "\nclothes worn\n";
        } else {
            player.wgt_gain(-clothes[ident - 1].get_wgt());
            std::cout << "wgt player is more than max, try to throw out some clothes";
        }
    }
}

void Map::throw_1() {
    std::string str;
    std::cin >> str;
    unsigned int j = 0;
    int pl;
    for (unsigned int i = 0; i < player_clothes.size(); ++i) {
        if (str == player_clothes[i].first) {
            j = i;
            player.arm_gain(-name_def_cl(str).get_arm());
            pl = clothes[player_clothes[i].second].get_wgt();
            player.wgt_gain(-pl);
            std::cout << "\nthe " << str << " is thrown out\n";
        }
    }
    for (unsigned int i = 0; i < player_clothes.size(); ++i) {
        if (i > j) {
            player_clothes[i-1] = player_clothes[i];
        }
    }
    player_clothes.resize(player_clothes.size() - 1);
}
