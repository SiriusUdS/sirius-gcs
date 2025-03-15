#ifndef SERIALTEST_H
#define SERIALTEST_H

#include "Logging.h"

#include <ceserial.h>

class SerialTest {
public:
    void init() {
        if (com.IsOpened()) {
            GCS_LOG_WARN("Serial port already open");
            return;
        }

        com.SetPortName("\\\\.\\COM3");
        if (com.Open() != 0) {
            GCS_LOG_WARN("Unsuccessful serial port init");
        } else {
            GCS_LOG_INFO("Successful serial port init");
        }
    }

    void readChar() {
        bool successFlag;
        char c = com.ReadChar(successFlag);
        if (successFlag && msgReadBufIdx < 1000) {
            msgReadBuf[msgReadBufIdx++] = c;
        }
    }

    bool readMessageReady() {
        return msgReadReady;
    }

    std::string consumeReadMessage() {
        if (!msgReadReady) {
            return "";
        }
        std::string msg = msgReadBuf;
        msgReadBuf[0] = 0;
        msgReadBufIdx = 0;
        msgReadReady = false;
    }

    void performWriteTest() {
        if (!com.IsOpened()) {
            GCS_LOG_WARN("Couldn't perform write test, serial port not open");
            return;
        }

        char s[] = "Bonjour ceci est un test!\n";
        bool successFlag = com.Write(s);

        GCS_LOG_INFO("Write to COM state: {}", successFlag);
    }

    void shutdown() {
        com.Close();
        GCS_LOG_INFO("Closed serial port");
    }

private:
    ceSerial com;
    char msgReadBuf[1000]{};
    int msgReadBufIdx{};
    bool msgReadReady{};
};

#endif // SERIALTEST_H
