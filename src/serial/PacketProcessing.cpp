#include "PacketProcessing.h"

#include "Accelerometer/AccelerometerPacket.h"
#include "Altimeter/AltimeterPacket.h"
#include "GPS/GpsPacket.h"
#include "Gyroscope/GyroscopePacket.h"
#include "LoadCell.h"
#include "Magnetometer/MagnetometerPacket.h"
#include "PlotDataCenter.h"
#include "PressureSensor/PressureSensorPacket.h"
#include "Rocket/RocketPacket.h"
#include "SerialTask.h"
#include "TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorPacket.h"
#include "Valve/ValvePacket.h"
#include "PressureTransducer.h"

namespace PacketProcessing {
size_t packetSize{};
uint8_t packetBuf[Constants::RECV_PACKET_MAX_SIZE];
} // namespace PacketProcessing

bool PacketProcessing::processIncomingPacket() {
    packetSize = SerialTask::com.nextPacketSize();

    if (packetSize == 0) {
        // No available packets
        return false;
    } else if (packetSize < sizeof(TelemetryHeader)) {
        GCS_LOG_WARN("PacketProcessing: Received packet size too small to fit header, ignoring packet.");
        SerialTask::com.dumpNextPacket();
        return false;
    } else if (packetSize > Constants::RECV_PACKET_MAX_SIZE) {
        GCS_LOG_WARN("PacketProcessing: Received packet size too big to fit in packet bufferk, ignoring packet");
        SerialTask::com.dumpNextPacket();
        return false;
    }

    if (SerialTask::com.getPacket(packetBuf) != packetSize) {
        GCS_LOG_WARN("PacketProcessing: The size of the packet that was just read does not match the expected packet size, ignoring packet.");
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) packetBuf;
    switch (header->bits.type) {
    case TELEMETRY_HEADER_TYPE_TELEMETRY:
        return processTelemetryPacket();
    case TELEMETRY_HEADER_TYPE_STATUS:
        return processStatusPacket();
    }

    GCS_LOG_WARN("PacketProcessing: Unknown packet type, ignoring packet.");
    return false;
}

bool PacketProcessing::processTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;
    float timeStamp = (float) packet->fields.timestamp_ms;

    PlotDataCenter::Thermistor1PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[0]));
    PlotDataCenter::Thermistor2PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[1]));
    PlotDataCenter::Thermistor3PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[2]));
    PlotDataCenter::Thermistor4PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[3]));
    PlotDataCenter::Thermistor5PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[4]));
    PlotDataCenter::Thermistor6PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[5]));
    PlotDataCenter::Thermistor7PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[6]));
    PlotDataCenter::Thermistor8PlotData.addData(timeStamp, TemperatureSensor::convertToTemperature((float) packet->fields.adcValues[7]));

    // PlotDataCenter::ADC9PlotData.addData(timeStamp, (float) packet->fields.adcValues[8]);
    // PlotDataCenter::ADC10PlotData.addData(timeStamp, (float) packet->fields.adcValues[9]);

    PlotDataCenter::PressureSensor1PlotData.addData(timeStamp, (float) packet->fields.adcValues[10]);
    PlotDataCenter::PressureSensor2PlotData.addData(timeStamp, (float) packet->fields.adcValues[11]);

    // PlotDataCenter::ADC13PlotData.addData(timeStamp, (float) packet->fields.adcValues[12]);
    // PlotDataCenter::ADC14PlotData.addData(timeStamp, (float) packet->fields.adcValues[13]);

    PlotDataCenter::LoadCell1PlotData.addData(timeStamp, LoadCell::convertRawToForce((float) packet->fields.adcValues[14]));
    PlotDataCenter::LoadCell2PlotData.addData(timeStamp, LoadCell::convertRawToForce((float) packet->fields.adcValues[15]));
    return true;
}

bool PacketProcessing::processStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    EngineStatusPacket* packet = (EngineStatusPacket*) packetBuf;
    return true;
}

bool PacketProcessing::validateIncomingPacketSize(size_t targetPacketSize, const char* packetName) {
    if (packetSize != targetPacketSize) {
        if (!SerialTask::com.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: process{}() called, but there's no packet to process.", packetName);
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid {} size, ignoring packet.", packetName);
        return false;
    }
    return true;
}