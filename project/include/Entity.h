#ifndef PROJECT_INCLUDE_ENTITY_H_
#define PROJECT_INCLUDE_ENTITY_H_


class Entity {
 public:
    Entity();
    Entity(int hp, int dmg);
    ~Entity() = default;

    bool is_alive();
    int kick(int dmg);
    int get_value_dmg();
    int get_value_hp();

 private:
    int hp;
    int dmg;
};

#endif  // PROJECT_INCLUDE_ENTITY_H_
