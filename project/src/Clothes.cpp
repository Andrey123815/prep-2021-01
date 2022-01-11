#include "Clothes.h"
#include <string>

Clothes::Clothes(): name() {}

Clothes::Clothes(std::string _name, int _wgt, int _arm): name(_name), wgt(_wgt), arm(_arm) {}

Armor::Armor():Clothes("armor", 3, 3) {}

Helmet::Helmet(): Clothes("helmet", 2, 3) {}

Shield::Shield(): Clothes("shield", 7, 5) {}

Pants::Pants(): Clothes("pants", 1, 1) {}

TShirt::TShirt(): Clothes("T-Shirt", 1, 1) {}
