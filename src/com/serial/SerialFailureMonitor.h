#ifndef SERIALFAILUREMONITOR_H
#define SERIALFAILUREMONITOR_H

class SerialFailureMonitor {
public:
    void trackRead(bool successful);
    void trackWrite(bool successful);
    bool isComWorking();
    void reset();

    static constexpr size_t CONSECUTIVE_FAILED_READS_BEFORE_FAILURE = 100;
    static constexpr size_t CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE = 100;

private:
    size_t consecutiveReadsFailed{};
    size_t consecutiveWritesFailed{};
};

#endif // SERIALFAILUREMONITOR_H
