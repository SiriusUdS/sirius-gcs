#ifndef SERIALSTATEMONITOR_H
#define SERIALSTATEMONITOR_H

#include "Timer.h"

#include <chrono>

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
        WORKING,     ///< Communication is working
        NOT_WORKING, ///< Communication is not working
    };
    void trackRead(bool successful);
    void trackWrite(bool successful);
    void trackSuccessfulPacketRead();
    void reset();
    State getState() const;

    // clang-format off
    static constexpr size_t CONSECUTIVE_FAILED_READS_BEFORE_FAILURE = 10; ///< Number of consecutive failed reads before considering communication as failing.
    static constexpr size_t CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE = 10; ///< Number of consecutive failed writes before considering communication as failing.
    static constexpr size_t TIME_WITHOUT_SUCCESSFUL_PACKET_READ_BEFORE_FAILURE_SEC = 3; ///< Amount of time without packet received before considering communication as failing.
    static constexpr size_t TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC = 5; ///< Amount of time without first packet received since connection before considering communication as failing.
    // clang-format on

private:
    bool ioSuccessSinceStart{};
    size_t consecutiveReadsFailed{};
    size_t consecutiveWritesFailed{};
    Timer lastSuccessfulPacketReadTimer;
};

#endif // SERIALSTATEMONITOR_H
