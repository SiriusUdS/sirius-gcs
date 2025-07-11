#ifndef COMDISCOVERY_H
#define COMDISCOVERY_H

#include <string>
#include <vector>

class ComDiscovery {
public:
    virtual void getAvailableComPorts(std::vector<std::string>& comPortVec) = 0;
};

#endif // COMDISCOVERY_H