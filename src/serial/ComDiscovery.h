#ifndef COMDISCOVERY_H
#define COMDISCOVERY_H

#include "Constants.h"

#include <Windows.h>
#include <string>
#include <vector>

void getAvailableComPorts(std::vector<std::string>& comPortVec) {
    comPortVec.clear();
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        WCHAR valueName[Constants::WIN_REG_VAL_BUF_SIZE], comPort[Constants::WIN_REG_VAL_BUF_SIZE];
        DWORD valueNameSize, comPortSize, index = 0, type;
        while (true) {
            valueNameSize = sizeof(valueName) / sizeof(WCHAR);
            comPortSize = sizeof(comPort);
            if (RegEnumValue(hKey, index++, valueName, &valueNameSize, nullptr, &type, (LPBYTE) comPort, &comPortSize) != ERROR_SUCCESS) {
                break;
            }
            comPortVec.emplace_back(&comPort[0], &comPort[Constants::WIN_REG_VAL_BUF_SIZE - 1]);
        }
        RegCloseKey(hKey);
    }
}

#endif // COMDISCOVERY_H
