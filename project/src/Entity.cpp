#include "Entity.h"

Entity::Entity() : hp(100), dmg(1) {}

Entity::Entity(int _hp, int _dmg) : hp(_hp), dmg(_dmg) {}

int Entity::get_value_dmg() {
    return this->dmg;
}
int Entity::get_value_hp() {
    return this->hp;
}

int Entity::kick(int damage) {
    return this->hp -= damage;
}

bool Entity::is_alive() {
    return this->hp > 0;
}
