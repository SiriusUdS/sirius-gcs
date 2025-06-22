#ifndef SERIALSTATEMONITOR_H
#define SERIALSTATEMONITOR_H

/**
 * @class SerialStateMonitor
 * @brief Monitors the state of the serial communication.
 */
class SerialStateMonitor {
public:
    /**
     * @enum State
     * @brief Represents the state of the serial communication.
     */
    enum State {
        STARTING,    ///< Communication is starting
        RESETTING,   ///< Communication is being reset
        WORKING,     ///< Communication is working
        NOT_WORKING, ///< Communication is not working
    };
    void trackRead(bool successful);
    void trackWrite(bool successful);
    State getState();
    void reset();

    // clang-format off
    static constexpr size_t CONSECUTIVE_FAILED_READS_BEFORE_FAILURE = 10; ///< Number of consecutive failed reads before considering communication as failing.
    static constexpr size_t CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE = 10; ///< Number of consecutive failed writes before considering communication as failing.
    // clang-format on

private:
    bool ioSuccessSinceStart{};
    bool hasReset{};
    size_t consecutiveReadsFailed{};
    size_t consecutiveWritesFailed{};
};

#endif // SERIALSTATEMONITOR_H
