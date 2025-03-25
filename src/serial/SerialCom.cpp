#include "SerialCom.h"

void SerialCom::init() {
    if (com.IsOpened()) {
        return;
    }

    com.SetPortName("\\\\.\\COM3");
    com.Open();
}

void SerialCom::getAvailableComPorts() {
    availableComPorts.clear();
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
            availableComPorts.emplace_back(&comPort[0], &comPort[255]);
        }
        RegCloseKey(hKey);
    }
}

void SerialCom::readChar() {
    if (!com.IsOpened()) {
        return;
    }

    bool successFlag;
    char c = com.ReadChar(successFlag);
    if (successFlag) {
        recvBuf.writeChar(c);
    }
}

size_t SerialCom::getPacket(char* recv) {
    return recvBuf.readPacket(recv);
}

bool SerialCom::write(char* msg, size_t size) {
    if (!com.IsOpened()) {
        return false;
    }

    return com.Write(msg, (long) size);
}

void SerialCom::shutdown() {
    com.Close();
}
