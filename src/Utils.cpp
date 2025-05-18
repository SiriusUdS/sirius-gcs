#include "Utils.h"

void Utils::convertStringToIniId(std::string& s) {
    for (auto& c : s) {
        if (c == ' ') {
            c = '_';
        } else {
            c = std::tolower(c);
        }
    }
}
