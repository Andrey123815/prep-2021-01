#include "Player.h"
#include <iostream>

Player::Player() : i(0), j(0), wgt(0), arm(0) {}

int Player::get_x() {
    return this->j;
}

int Player::get_y() {
    return this->i;
}

void Player::moving_player(int y, int x) {
    this->j += x;
    this->i += y;
}

int Player::get_arm() {
    return arm;
}

int Player::get_wgt() {
    return wgt;
}

int Player::arm_gain(int _arm) {
    return arm += _arm;
}

int Player::wgt_gain(int _wgt) {
    return wgt += _wgt;
}
