#pragma once

#ifndef NHANVAT_H
#define NHANVAT_H

#include <string>

class NhanVat {
protected:
    std::string name;
    int level;
    std::string element;

public:
    NhanVat(const std::string& n, int lvl, const std::string& elem);
    virtual int getDamage() const = 0;
    std::string getElement() const;
    int getLevel() const;
    std::string getName() const;
    virtual ~NhanVat() = default;
};

#endif // NHANVAT_H
