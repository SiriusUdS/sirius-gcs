#ifndef COMDISCOVERYWINDOWS_H
#define COMDISCOVERYWINDOWS_H

#include "ComDiscovery.h"

#include <Windows.h>

class ComDiscoveryWindows : public ComDiscovery {
public:
    void getAvailableComPorts(std::vector<std::string>& comPortVec) override;

private:
    std::string wcharToString(const WCHAR* wstr);
};

#endif // COMDISCOVERYWINDOWS_H
