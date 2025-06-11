#ifndef SERIALFAILUREMONITOR_H
#define SERIALFAILUREMONITOR_H

class SerialFailureMonitor {
public:
    void trackRead(bool successful);
    void trackWrite(bool successful);
    bool isComWorking();
    void reset();

private:
    size_t consecutiveReadsFailed{};
    size_t consecutiveWritesFailed{};
};

#endif // SERIALFAILUREMONITOR_H
