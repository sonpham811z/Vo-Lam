#pragma once
#ifndef NGUHANH_H
#define NGUHANH_H

#include <string>

class NguHanh {
public:
    static const std::string hanh[];
    static bool isSinh(const std::string& e1, const std::string& e2);
    static bool isKhac(const std::string& e1, const std::string& e2);
};

#endif // NGUHANH_H
