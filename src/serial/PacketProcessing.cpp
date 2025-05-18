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
    PlotDataCenter::ADC15PlotData.addData(timeStamp, TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[14]));
    PlotDataCenter::ADC16PlotData.addData(timeStamp, TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[15]));

    PlotDataCenter::LoadCell1Data.addData(timeStamp, TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[14]));
    PlotDataCenter::LoadCell2Data.addData(timeStamp, TEMP_LOAD_CELL_CONVERSION((float) packet->fields.adcValues[15]));
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
