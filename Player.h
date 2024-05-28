#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public NhanVat {
private:
    std::string sect;

public:
    Player(const std::string& n, const std::string& s, int lvl);
    int getDamage() const override;
    std::string getSect() const;
    
};

#endif // PLAYER_H

