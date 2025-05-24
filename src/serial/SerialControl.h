#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <chrono>

namespace SerialControl {
void startComIfNeeded();
void readIncomingBytesAtSetRate();

extern std::chrono::time_point<std::chrono::steady_clock> lastSerialConnectionAttempt;
extern std::chrono::time_point<std::chrono::steady_clock> lastSerialReadTime;
} // namespace SerialControl

#endif SERIALCONTROL_H
