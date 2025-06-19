#ifndef SERIALFAILUREMONITOR_H
#define SERIALFAILUREMONITOR_H

/**
 * @class SerialFailureMonitor
 * @brief Monitors the failed read/write serial operations to determine if the serial communication is failing.
 */
class SerialFailureMonitor {
public:
    void trackRead(bool successful);
    void trackWrite(bool successful);
    bool isComFailing();
    void reset();

    // clang-format off
    static constexpr size_t CONSECUTIVE_FAILED_READS_BEFORE_FAILURE = 100; ///< Number of consecutive failed reads before considering communication as failing.
    static constexpr size_t CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE = 100; ///< Number of consecutive failed writes before considering communication as failing.
    // clang-format on

private:
    size_t consecutiveReadsFailed{};
    size_t consecutiveWritesFailed{};
};

#endif // SERIALFAILUREMONITOR_H
