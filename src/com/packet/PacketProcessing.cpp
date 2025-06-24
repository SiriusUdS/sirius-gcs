#include "PacketProcessing.h"

#include "GSDataCenter.h"
#include "LoadCell.h"
#include "Logging.h"
#include "PacketReceiver.h"
#include "PlotData.h"
#include "PressureTransducer.h"
#include "SerialCom.h"
#include "SerialTask.h"
#include "SwitchData.h"
#include "Telecommunication/TelemetryPacket.h"
#include "TemperatureSensor.h"

namespace PacketProcessing {
constexpr size_t MAX_PACKET_SIZE = 64;

size_t packetSize{};
uint8_t packetBuf[MAX_PACKET_SIZE];
} // namespace PacketProcessing

bool PacketProcessing::processIncomingPacket() {
    packetSize = SerialTask::packetReceiver.nextPacketSize();

    if (packetSize == 0) {
        // No available packets
        return false;
    } else if (packetSize < sizeof(TelemetryHeader)) {
        GCS_LOG_WARN("PacketProcessing: Received packet size ({}) too small to fit header ({}), ignoring packet.", packetSize,
                     sizeof(TelemetryHeader));
        dumpNextPacket("IncomingPacket");
        return false;
    } else if (packetSize > MAX_PACKET_SIZE) {
        GCS_LOG_WARN("PacketProcessing: Received packet size ({}) too big to fit in packet buffer ({}), ignoring packet", packetSize,
                     MAX_PACKET_SIZE);
        dumpNextPacket("IncomingPacket");
        return false;
    }

    if (!SerialTask::com.getPacket(packetBuf)) {
        GCS_LOG_ERROR("PacketProcessing: Something went wrong while getting the next packet.");
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) packetBuf;
    switch (header->bits.type) {
    case TELEMETRY_TYPE_CODE:
        return processEngineTelemetryPacket();
    case STATUS_TYPE_CODE:
        if (header->bits.boardId == TELEMETRY_GS_CONTROL_BOARD_ID) {
            return processGSControlPacket();
        } else if (header->bits.boardId == TELEMETRY_ENGINE_BOARD_ID) {
            return processEngineStatusPacket();
        } else {
            GCS_LOG_WARN("PacketProcessing: Status packet contains invalid boardId, ignoring packet.");
            dumpNextPacket("IncomingPacket");
            return false;
        }
    }

    GCS_LOG_WARN("PacketProcessing: Unknown packet type, ignoring packet.");
    return false;
}

bool PacketProcessing::processEngineTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;
    float timeStamp = (float) packet->fields.timestamp_ms;

    constexpr size_t THERMISTOR_ADC_INDEX_OFFSET = 0;
    for (size_t plotIdx = 0; plotIdx < GSDataCenter::THERMISTOR_AMOUNT; plotIdx++) {
        const size_t adcIdx = plotIdx + THERMISTOR_ADC_INDEX_OFFSET;
        const float temperature = TemperatureSensor::adcToTemperature(packet->fields.adcValues[adcIdx]);
        GSDataCenter::ThermistorPlotData[plotIdx].addData(timeStamp, temperature);
    }

    constexpr size_t PRESSURE_SENSOR_ADC_INDEX_OFFSET = 10;
    for (size_t plotIdx = 0; plotIdx < GSDataCenter::PRESSURE_SENSOR_AMOUNT; plotIdx++) {
        const size_t adcIdx = plotIdx + PRESSURE_SENSOR_ADC_INDEX_OFFSET;
        const float pressure = PressureTransducer::adcToPressure(packet->fields.adcValues[adcIdx], 3);
        GSDataCenter::PressureSensorPlotData[plotIdx].addData(timeStamp, pressure);
    }

    constexpr size_t LOAD_CELL_ADC_INDEX_OFFSET = 14;
    for (size_t plotIdx = 0; plotIdx < GSDataCenter::LOAD_CELL_AMOUNT; plotIdx++) {
        const size_t adcIdx = plotIdx + LOAD_CELL_ADC_INDEX_OFFSET;
        const float force = LoadCell::adcToForce((float) packet->fields.adcValues[adcIdx]);
        GSDataCenter::LoadCellPlotData[plotIdx].addData(timeStamp, force);
    }

    return true;
}

bool PacketProcessing::processGSControlPacket() {
    if (!validateIncomingPacketSize(sizeof(GSControlStatusPacket), "GSControlStatusPacket")) {
        return false;
    }

    GSControlStatusPacket* packet = (GSControlStatusPacket*) packetBuf;

    GSDataCenter::AllowDumpSwitchData.isOn = packet->fields.status.bits.isAllowDumpSwitchOn;
    GSDataCenter::AllowFillSwitchData.isOn = packet->fields.status.bits.isAllowFillSwitchOn;
    GSDataCenter::ArmIgniterSwitchData.isOn = packet->fields.status.bits.isArmIgniterSwitchOn;
    GSDataCenter::ArmServoSwitchData.isOn = packet->fields.status.bits.isArmServoSwitchOn;
    GSDataCenter::EmergencyStopButtonData.isOn = packet->fields.status.bits.isEmergencyStopButtonPressed;
    GSDataCenter::FireIgniterButtonData.isOn = packet->fields.status.bits.isFireIgniterButtonPressed;
    GSDataCenter::UnsafeKeySwitchData.isOn = packet->fields.status.bits.isUnsafeKeySwitchPressed;
    GSDataCenter::ValveStartButtonData.isOn = packet->fields.status.bits.isValveStartButtonPressed;

    return true;
}

bool PacketProcessing::processEngineStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    EngineStatusPacket* packet = (EngineStatusPacket*) packetBuf;
    return true;
}

bool PacketProcessing::validateIncomingPacketSize(size_t targetPacketSize, const char* packetName) {
    if (packetSize != targetPacketSize) {
        if (dumpNextPacket(packetName)) {
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid {} size, ignoring packet.", packetName);
        return false;
    }
    return true;
}

bool PacketProcessing::dumpNextPacket(const char* packetName) {
    if (!SerialTask::packetReceiver.dumpNextPacket()) {
        GCS_LOG_WARN("PacketProcessing: process{}() called, but there's no packet to process.", packetName);
        return false;
    }
    return true;
}
