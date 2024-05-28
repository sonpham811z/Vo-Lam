#include "Player.h"
#include "NguHanh.h"

const std::string sectInfoList[] = {
    "Thieu Lam", "Minh Giao", "Ngu Doc", "Duong Mon",
    "Nga My", "Thuy Yen", "Cai Bang", "Thien Nhan",
    "Con Lon", "Vo Dang"
};

const std::string sectElementList[] = {
    "Kim", "Kim", "Moc", "Moc", "Thuy", "Thuy",
    "Hoa", "Hoa", "Tho", "Tho"
};

std::string getElementBySect(const std::string& sect) {
    for (size_t i = 0; i < sizeof(sectInfoList) / sizeof(std::string); ++i) {
        if (sectInfoList[i] == sect) {
            return sectElementList[i];
        }
    }
    return "";
}

Player::Player(const std::string& n, const std::string& s, int lvl) : NhanVat(n, lvl, getElementBySect(s)), sect(s) {}

int Player::getDamage() const {
    return level * 5;
}

std::string Player::getSect() const {
    return sect;
}


