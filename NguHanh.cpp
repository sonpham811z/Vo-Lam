#include "NguHanh.h"

const std::string NguHanh::hanh[] = { "Kim", "Moc", "Thuy", "Hoa", "Tho" };

bool NguHanh::isSinh(const std::string& e1, const std::string& e2) {
    return (e1 == "Hoa" && e2 == "Tho") ||
        (e1 == "Tho" && e2 == "Kim") ||
        (e1 == "Kim" && e2 == "Thuy") ||
        (e1 == "Thuy" && e2 == "Moc") ||
        (e1 == "Moc" && e2 == "Hoa");
}

bool NguHanh::isKhac(const std::string& e1, const std::string& e2) {
    return (e1 == "Hoa" && e2 == "Kim") ||
        (e1 == "Tho" && e2 == "Thuy") ||
        (e1 == "Kim" && e2 == "Moc") ||
        (e1 == "Thuy" && e2 == "Hoa") ||
        (e1 == "Moc" && e2 == "Tho");
}
