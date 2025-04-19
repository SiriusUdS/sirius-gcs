#ifndef PACKETPROCESSING_H
#define PACKETPROCESSING_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

namespace PacketProcessing {
bool processIncomingPacket();

bool processAccelerometerPacket();
bool processGyroscopePacket();
bool processAltimeterPacket();
bool processGpsPacket();
bool processMagnetometerPacket();
bool processPressureSensorPacket();
bool processRocketPacket();
bool processTemperatureSensorPacket();
bool processValvePacket();

bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
