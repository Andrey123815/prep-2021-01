#include "Enemy.h"
#include <iostream>

Enemy::Enemy() : name() {}

Enemy::Enemy(std::string _name, int _hp, int _dmg): Entity(_hp, _dmg), name(_name) {}

Wolf::Wolf():Enemy("wolf", 6, 11) {}

Dog::Dog(): Enemy("dog", 3, 6) {}

Rat::Rat(): Enemy("rat", 2, 4) {}
