#ifndef COMPORTSELECTOR_H
#define COMPORTSELECTOR_H

#include <string>
#include <vector>

class ComPortSelector {
public:
    std::string current() const;
    void next();

private:
    size_t currentComPortIdx{};
    std::vector<std::string> comPorts;
};

#endif // COMPORTSELECTOR_H
