#ifndef ABSTRACTCOMPORTDISCOVERY_H
#define ABSTRACTCOMPORTDISCOVERY_H

#include <string>
#include <vector>

class AbstractComPortDiscovery {
public:
    virtual void getAvailableComPorts(std::vector<std::string>& comPortVec) = 0;
};

#endif // ABSTRACTCOMPORTDISCOVERY_H
