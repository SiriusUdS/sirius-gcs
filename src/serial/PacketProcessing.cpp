#include "PacketProcessing.h"

#include "Accelerometer/AccelerometerPacket.h"
#include "Altimeter/AltimeterPacket.h"
#include "GPS/GpsPacket.h"
#include "Gyroscope/GyroscopePacket.h"
#include "Magnetometer/MagnetometerPacket.h"
#include "PlotDataCenter.h"
#include "PressureSensor/PressureSensorPacket.h"
#include "Rocket/RocketPacket.h"
#include "SerialTask.h"
#include "TemperatureSensor/TemperatureSensorPacket.h"
#include "Valve/ValvePacket.h"

namespace PacketProcessing {
size_t packetSize{};
uint8_t packetBuf[Constants::RECV_PACKET_MAX_SIZE];
} // namespace PacketProcessing

float TEMP_LOAD_CELL_CONVERSION(float adcValue) {
    if (adcValue < 0 || adcValue > 4095.0) {
        GCS_LOG_ERROR("Load cell ADC value out of range!");
        return -1;
    }

    float force = 200 * ((adcValue * 3.3f / 4096.f) / 209.f) / 0.015; // / 209.f;

    GCS_LOG_INFO("Load Cell Force: {} N", force);
    return force;
}

bool PacketProcessing::processIncomingPacket() {
    // TODO: MAKE VERIFICATIONS

    packetSize = SerialTask::com.getPacket(packetBuf);
    TelemetryHeader* header = (TelemetryHeader*) packetBuf;

    // GCS_LOG_TRACE("Try reading packet");

    if (!packetSize) {
        // No available packets
        return false;
    }

    // GCS_LOG_TRACE("Packet detected");

    switch (header->bits.type) {
    case TELEMETRY_HEADER_TYPE_TELEMETRY:
        return processTelemetryPacket();
    case TELEMETRY_HEADER_TYPE_STATUS:
        return processStatusPacket();
    }

    // GCS_LOG_WARN("PacketProcessing: Unknown packet type, ignoring packet.");
    return false;
}

bool PacketProcessing::processTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;
    float timeStamp = (float) packet->fields.timestamp_ms;
    // float v1 = TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[14]);
    // float v2 = TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[15]);
    PlotDataCenter::ADC1PlotData.addData(timeStamp, (float) packet->fields.adcValues[0]);
    PlotDataCenter::ADC2PlotData.addData(timeStamp, (float) packet->fields.adcValues[1]);
    PlotDataCenter::ADC3PlotData.addData(timeStamp, (float) packet->fields.adcValues[2]);
    PlotDataCenter::ADC4PlotData.addData(timeStamp, (float) packet->fields.adcValues[3]);
    PlotDataCenter::ADC5PlotData.addData(timeStamp, (float) packet->fields.adcValues[4]);
    PlotDataCenter::ADC6PlotData.addData(timeStamp, (float) packet->fields.adcValues[5]);
    PlotDataCenter::ADC7PlotData.addData(timeStamp, (float) packet->fields.adcValues[6]);
    PlotDataCenter::ADC8PlotData.addData(timeStamp, (float) packet->fields.adcValues[7]);
    PlotDataCenter::ADC9PlotData.addData(timeStamp, (float) packet->fields.adcValues[8]);
    PlotDataCenter::ADC10PlotData.addData(timeStamp, (float) packet->fields.adcValues[9]);
    PlotDataCenter::ADC11PlotData.addData(timeStamp, (float) packet->fields.adcValues[10]);
    PlotDataCenter::ADC12PlotData.addData(timeStamp, (float) packet->fields.adcValues[11]);
    PlotDataCenter::ADC13PlotData.addData(timeStamp, (float) packet->fields.adcValues[12]);
    PlotDataCenter::ADC14PlotData.addData(timeStamp, (float) packet->fields.adcValues[13]);
    PlotDataCenter::ADC15PlotData.addData(timeStamp, (float) packet->fields.adcValues[14]);
    PlotDataCenter::ADC16PlotData.addData(timeStamp, TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[15]));
    return true;
}

bool PacketProcessing::processStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    EngineStatusPacket* packet = (EngineStatusPacket*) packetBuf;

    return true;
}

bool PacketProcessing::processAccelerometerPacket() {
    if (!validateIncomingPacketSize(sizeof(AccelerometerPacket), "AccelerometerPacket")) {
        return false;
    }

    AccelerometerPacket* packet = (AccelerometerPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawX = packet->fields.rawData.data.rawX;
    float rawY = packet->fields.rawData.data.rawY;
    float rawZ = packet->fields.rawData.data.rawZ;
    PlotDataCenter::AccelerometerXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::AccelerometerYPlotData.addData(timeStamp, rawY);
    PlotDataCenter::AccelerometerZPlotData.addData(timeStamp, rawZ);
    return true;
}

bool PacketProcessing::processGyroscopePacket() {
    if (!validateIncomingPacketSize(sizeof(GyroscopePacket), "GyroscopePacket")) {
        return false;
    }

    GyroscopePacket* packet = (GyroscopePacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawX = packet->fields.rawData.data.rawX;
    float rawY = packet->fields.rawData.data.rawY;
    float rawZ = packet->fields.rawData.data.rawZ;
    PlotDataCenter::GyroscopeXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::GyroscopeYPlotData.addData(timeStamp, rawY);
    PlotDataCenter::GyroscopeZPlotData.addData(timeStamp, rawZ);
    return true;
}

bool PacketProcessing::processAltimeterPacket() {
    if (!validateIncomingPacketSize(sizeof(AltimeterPacket), "AltimeterPacket")) {
        return false;
    }

    AltimeterPacket* packet = (AltimeterPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawAltitude = packet->fields.rawData.data.rawAltitude;
    PlotDataCenter::AltimeterPlotData.addData(timeStamp, rawAltitude);
    return true;
}

bool PacketProcessing::processGpsPacket() {
    if (!validateIncomingPacketSize(sizeof(GpsPacket), "GpsPacket")) {
        return false;
    }

    GpsPacket* packet = (GpsPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawLongitude = packet->fields.rawData.data.rawLongitude;
    float rawLatitude = packet->fields.rawData.data.rawLatitude;
    PlotDataCenter::GpsLongitudePlotData.addData(timeStamp, rawLongitude);
    PlotDataCenter::GpsLatitudePlotData.addData(timeStamp, rawLatitude);
    return true;
}

bool PacketProcessing::processMagnetometerPacket() {
    if (!validateIncomingPacketSize(sizeof(MagnetometerPacket), "MagnetometerPacket")) {
        return false;
    }

    MagnetometerPacket* packet = (MagnetometerPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawX = packet->fields.rawData.data.rawX;
    float rawY = packet->fields.rawData.data.rawY;
    float rawZ = packet->fields.rawData.data.rawZ;
    PlotDataCenter::MagnetometerXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::MagnetometerYPlotData.addData(timeStamp, rawX);
    PlotDataCenter::MagnetometerZPlotData.addData(timeStamp, rawX);
    return true;
}

bool PacketProcessing::processPressureSensorPacket() {
    if (!validateIncomingPacketSize(sizeof(PressureSensorPacket), "PressureSensorPacket")) {
        return false;
    }

    PressureSensorPacket* packet = (PressureSensorPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawPressure = packet->fields.rawData.data.rawPressure;
    PlotDataCenter::PressureSensorPlotData.addData(timeStamp, rawPressure);
    return true;
}

bool PacketProcessing::processRocketPacket() {
    if (!validateIncomingPacketSize(sizeof(RocketPacket), "RockerPacket")) {
        return false;
    }

    RocketPacket* packet = (RocketPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    PlotDataCenter::RocketPlotData.addData(timeStamp, 0);
    return true;
}

bool PacketProcessing::processTemperatureSensorPacket() {
    if (!validateIncomingPacketSize(sizeof(TemperatureSensorPacket), "TemperatureSensorPacket")) {
        return false;
    }

    TemperatureSensorPacket* packet = (TemperatureSensorPacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float rawTemperature = packet->fields.rawData.data.rawTemperature;
    PlotDataCenter::TemperatureSensorPlotData.addData(timeStamp, rawTemperature);
    return true;
}

bool PacketProcessing::processValvePacket() {
    if (!validateIncomingPacketSize(sizeof(ValvePacket), "ValvePacket")) {
        return false;
    }

    ValvePacket* packet = (ValvePacket*) packetBuf;
    float timeStamp = packet->fields.rawData.timeStamp_ms;
    float status = packet->fields.rawData.status.bits.state;
    PlotDataCenter::ValvePlotData.addData(timeStamp, status);
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
