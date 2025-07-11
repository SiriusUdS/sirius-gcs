#include "ComDiscovery.h"

namespace ComDiscovery {
std::string wcharToString(const WCHAR* wstr);
}

void ComDiscovery::getAvailableComPorts(std::vector<std::string>& comPortVec) {
    static constexpr size_t WIN_REG_VAL_BUF_SIZE = 256;

    comPortVec.clear();
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        WCHAR valueName[WIN_REG_VAL_BUF_SIZE], comPort[WIN_REG_VAL_BUF_SIZE];
        DWORD valueNameSize, comPortSize, index = 0, type;
        while (true) {
            valueNameSize = sizeof(valueName) / sizeof(WCHAR);
            comPortSize = sizeof(comPort);
            if (RegEnumValue(hKey, index++, valueName, &valueNameSize, nullptr, &type, (LPBYTE) comPort, &comPortSize) != ERROR_SUCCESS) {
                break;
            }

            comPortVec.push_back("\\\\.\\" + wcharToString(comPort));
        }
        RegCloseKey(hKey);
    }
}

std::string ComDiscovery::wcharToString(const WCHAR* wstr) {
    if (wstr == nullptr)
        return {};

    // Get the length of the resulting string
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed == 0)
        return {};

    std::vector<char> buffer(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer.data(), size_needed, nullptr, nullptr);

    return std::string(buffer.data());
}
