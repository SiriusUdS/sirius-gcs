#ifndef WINDOWSCOMPORTDISCOVERY_H
#define WINDOWSCOMPORTDISCOVERY_H

#include "AbstractComPortDiscovery.h"

#include <Windows.h>

class WindowsComPortDiscovery : public AbstractComPortDiscovery {
public:
    void getAvailableComPorts(std::vector<std::string>& comPortVec) override;

private:
    std::string wcharToString(const WCHAR* wstr); // TODO: this shouldn't be a member function of this class
};

#endif // WINDOWSCOMPORTDISCOVERY_H
