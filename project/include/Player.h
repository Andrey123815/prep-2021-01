#ifndef PROJECT_INCLUDE_PLAYER_H_
#define PROJECT_INCLUDE_PLAYER_H_
#include <iostream>
#include <string>
#include "Entity.h"

class Player: public Entity {
 public:
    Player();
    ~Player() = default;

    int get_x();
    int get_y();
    void moving_player(int y, int x);
    int get_arm();
    int get_wgt();
    int arm_gain(int _arm);
    int wgt_gain(int _wgt);

 private:
    int i;
    int j;
    int wgt;
    int arm;
};

#endif  // PROJECT_INCLUDE_PLAYER_H_
