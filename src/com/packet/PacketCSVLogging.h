#ifndef PACKETCSVLOGGING_H
#define PACKETCSVLOGGING_H

#include "CSVLogger.h"
#include "GSDataCenter.h"
#include "Telecommunication/TelemetryPacket.h"

namespace PacketCSVLogging {
void init();
void logEngineTelemetryPacket(float timestamp,
                              float thermistorValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD],
                              float pressureSensorValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD]);
void logFillingStationTelemetryPacket(float timestamp,
                                      float thermistorValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD],
                                      float pressureSensorValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD],
                                      float loadCellValues[GSDataCenter::LOAD_CELL_AMOUNT]);
void logGSControlPacket(const GSControlStatusPacket* packet);
void logEngineStatusPacket(const EngineStatusPacket* packet);
void logFillingStationStatusPacket(const FillingStationStatusPacket* packet);
} // namespace PacketCSVLogging

#endif // PACKETCSVLOGGING_H
