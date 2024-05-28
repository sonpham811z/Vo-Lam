#include "Quaivat.h"

Monster::Monster(const std::string& n, int lvl, bool boss, const std::string& elem) : NhanVat(n, lvl, elem), isBoss(boss) {}

int Monster::getDamage() const {
    return level * (isBoss ? 7 : 3);
}

bool Monster::isBossMonster() const {
    return isBoss;
}

