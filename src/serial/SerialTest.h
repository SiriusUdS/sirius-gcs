#ifndef SERIALTEST_H
#define SERIALTEST_H

#include "Logging.h"

#include <ceserial.h>

class SerialTest {
public:
    void init() {
        if (com.IsOpened()) {
            GCS_LOG_WARN("Serial Port Already Open");
            return;
        }

        com.SetPortName("\\\\.\\COM3");
        if (com.Open() != 0) {
            GCS_LOG_WARN("Unsuccessful Serial Port Init");
        } else {
            GCS_LOG_INFO("Successful Serial Port Init");
        }
    }

    void performTest() {
        if (!com.IsOpened()) {
            GCS_LOG_WARN("Couldn't perform test, serial port not open");
            return;
        }

        bool successFlag = false;
        int unsuccessfulReadCount = 0;
        char c = '\0';
        std::string message;
        while (c != ' ' && unsuccessfulReadCount <= 100) {
            if (successFlag) {
                message += c;
            } else {
                unsuccessfulReadCount++;
            }
            c = com.ReadChar(successFlag);
        }

        GCS_LOG_INFO("Read the following message from COM: {}", message);
    }

    void shutdown() {
        com.Close();
        GCS_LOG_INFO("Closed Serial Port");
    }

private:
    ceSerial com;
};

#endif // SERIALTEST_H
