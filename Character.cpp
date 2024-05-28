#include "Character.h"

NhanVat::NhanVat(const std::string& n, int lvl, const std::string& elem) : name(n), level(lvl), element(elem) {}

std::string NhanVat::getElement() const {
    return element;
}

int NhanVat::getLevel() const {
    return level;
}

std::string NhanVat::getName() const {
    return name;
}

