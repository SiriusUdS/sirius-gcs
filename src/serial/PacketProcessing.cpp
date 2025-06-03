#include "PacketProcessing.h"

#include "GSDataCenter.h"
#include "LoadCell.h"
#include "Logging.h"
#include "PlotData.h"
#include "PressureTransducer.h"
#include "SerialCom.h"
#include "SerialTask.h"
#include "SwitchData.h"
#include "Telecommunication/TelemetryPacket.h"
#include "TemperatureSensor.h"

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
        dumpNextPacket("IncomingPacket");
        return false;
    } else if (packetSize > Constants::RECV_PACKET_MAX_SIZE) {
        GCS_LOG_WARN("PacketProcessing: Received packet size too big to fit in packet bufferk, ignoring packet");
        dumpNextPacket("IncomingPacket");
        return false;
    }

    if (SerialTask::com.getPacket(packetBuf) != packetSize) {
        GCS_LOG_WARN("PacketProcessing: The size of the packet that was just read does not match the expected packet size, ignoring packet.");
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) packetBuf;
    switch (header->bits.type) {
    case TELEMETRY_TYPE_CODE:
        return processTelemetryPacket();
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

bool PacketProcessing::processTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;
    float timeStamp = (float) packet->fields.timestamp_ms;

    GSDataCenter::Thermistor1PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[0]));
    GSDataCenter::Thermistor2PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[1]));
    GSDataCenter::Thermistor3PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[2]));
    GSDataCenter::Thermistor4PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[3]));
    GSDataCenter::Thermistor5PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[4]));
    GSDataCenter::Thermistor6PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[5]));
    GSDataCenter::Thermistor7PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[6]));
    GSDataCenter::Thermistor8PlotData.addData(timeStamp, (float) TemperatureSensor::convertToTemperature(packet->fields.adcValues[7]));

    const float PRESSURE_SENSOR_1 = PressureTransducer::convertRawToPressure(packet->fields.adcValues[10], 3);
    const float PRESSURE_SENSOR_2 = PressureTransducer::convertRawToPressure(packet->fields.adcValues[11], 3);
    GSDataCenter::PressureSensor1PlotData.addData(timeStamp, PRESSURE_SENSOR_1);
    GSDataCenter::PressureSensor2PlotData.addData(timeStamp, PRESSURE_SENSOR_2);
    // GSDataCenter::PressureSensor1PlotData.addData(timeStamp, packet->fields.adcValues[10]);
    // GSDataCenter::PressureSensor2PlotData.addData(timeStamp, packet->fields.adcValues[11]);

    GSDataCenter::LoadCell1PlotData.addData(timeStamp, LoadCell::convertRawToForce((float) packet->fields.adcValues[14]));
    GSDataCenter::LoadCell2PlotData.addData(timeStamp, LoadCell::convertRawToForce((float) packet->fields.adcValues[15]));

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
    if (!SerialTask::com.dumpNextPacket()) {
        GCS_LOG_WARN("PacketProcessing: process{}() called, but there's no packet to process.", packetName);
        return false;
    }
    return true;
}
