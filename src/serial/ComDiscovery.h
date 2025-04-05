#ifndef COMDISCOVERY_H
#define COMDISCOVERY_H

#include <Windows.h>
#include <string>
#include <vector>

void getAvailableComPorts(std::vector<std::string>& comPortVec) {
    comPortVec.clear();
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        WCHAR valueName[256], comPort[256];
        DWORD valueNameSize, comPortSize, index = 0, type;
        while (true) {
            valueNameSize = sizeof(valueName) / sizeof(WCHAR);
            comPortSize = sizeof(comPort);
            if (RegEnumValue(hKey, index++, valueName, &valueNameSize, nullptr, &type, (LPBYTE) comPort, &comPortSize) != ERROR_SUCCESS) {
                break;
            }
            comPortVec.emplace_back(&comPort[0], &comPort[255]);
        }
        RegCloseKey(hKey);
    }
}

#endif // COMDISCOVERY_H
