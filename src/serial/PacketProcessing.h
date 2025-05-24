#ifndef PACKETPROCESSING_H
#define PACKETPROCESSING_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

namespace PacketProcessing {
bool processIncomingPacket();
bool processTelemetryPacket();
bool processStatusPacket();
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);

extern size_t packetSize;
extern uint8_t packetBuf[Constants::RECV_PACKET_MAX_SIZE];
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
