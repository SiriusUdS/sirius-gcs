#ifndef PACKETPROCESSING_H
#define PACKETPROCESSING_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

namespace PacketProcessing {
bool processPacket();

bool processAccelerometerPacket();
bool processGyroscopePacket();
bool processAltimeterPacket();
bool processGpsPacket();
bool processMagnetometerPacket();
bool processPressureSensorPacket();
bool processRocketPacket();
bool processTemperatureSensorPacket();
bool processValvePacket();

extern SerialCom serialCom;
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
