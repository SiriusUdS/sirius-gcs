#ifndef COMDISCOVERY_H
#define COMDISCOVERY_H

#include <Windows.h>
#include <string>
#include <vector>

namespace ComDiscovery {
void getAvailableComPorts(std::vector<std::string>& comPortVec);
} // namespace ComDiscovery

#endif // COMDISCOVERY_H
