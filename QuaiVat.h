#pragma once
#ifndef QUAIVAT_H
#define QUAIVAT_H

#include "Character.h"

class Monster : public NhanVat {
private:
    bool isBoss;

public:
    Monster(const std::string& n, int lvl, bool boss, const std::string& elem);
    int getDamage() const override;
    bool isBossMonster() const;
};

#endif // QUAIVAT_H

