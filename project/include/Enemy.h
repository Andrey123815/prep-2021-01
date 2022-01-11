#ifndef PROJECT_INCLUDE_ENEMY_H_
#define PROJECT_INCLUDE_ENEMY_H_
#include <string>
#include "../include/Entity.h"

class Enemy : public Entity {
 public:
    Enemy();
    Enemy(std::string _name, int _hp, int _dmg);
    ~Enemy() = default;

 private:
    std::string name;
    int hp;
    int dmg;
};

class Wolf: public Enemy {
 public:
    Wolf();
};

class Dog: public Enemy {
 public:
    Dog();
};

class Rat: public Enemy {
 public:
    Rat();
};

#endif  // PROJECT_INCLUDE_ENEMY_H_
