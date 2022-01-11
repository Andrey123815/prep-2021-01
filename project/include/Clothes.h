#ifndef PROJECT_INCLUDE_CLOTHES_H_
#define PROJECT_INCLUDE_CLOTHES_H_
#include <string>

class Clothes {
 public:
    Clothes();
    Clothes(std::string _name, int _wgt, int _arm);
    ~Clothes() = default;

    int get_wgt() {
        return this->wgt;
    }
    int get_arm() {
        return this->arm;
    }
 private:
    std::string name;
    int wgt;
    int arm;
};

class Armor : public Clothes {
 public:
    Armor();
};

class Helmet : public Clothes {
 public:
    Helmet();
};

class Shield : public Clothes {
 public:
    Shield();
};

class Pants : public Clothes {
 public:
    Pants();
};

class TShirt : public Clothes {
 public:
    TShirt();
};

#endif  // PROJECT_INCLUDE_CLOTHES_H_
