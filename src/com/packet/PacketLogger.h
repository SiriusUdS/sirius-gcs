#ifndef PACKETLOGGER_H
#define PACKETLOGGER_H

#include "GSControl/GSControlStatus.h"

namespace PacketLogger {
constexpr const char* GS_CONTROL_PACKET_LOG_TEMPLATE =
  "Allow Dump Switch: {}, Allow Fill Switch: {}, Arm Igniter Switch: {}, Arm Servo Switch: {}, Emergency "
  "Stop Button: {}, Fire Igniter Button: {}, Unsafe Key Switch: {}, Valve Start Button: {}";

void logGsControl(GSControlStatus* packet);
} // namespace PacketLogger

#endif // PACKETLOGGER_H
