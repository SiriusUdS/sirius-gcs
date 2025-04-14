#include "PacketProcessing.h"

#include "Accelerometer/AccelerometerPacket.h"
#include "Altimeter/AltimeterPacket.h"
#include "GPS/GpsPacket.h"
#include "Gyroscope/GyroscopePacket.h"
#include "Magnetometer/MagnetometerPacket.h"
#include "PlotDataCenter.h"
#include "PressureSensor/PressureSensorPacket.h"
#include "Rocket/RocketPacket.h"
#include "TemperatureSensor/TemperatureSensorPacket.h"
#include "Valve/ValvePacket.h"
#include "WordFormatter.h"

namespace PacketProcessing {
SerialCom serialCom;
} // namespace PacketProcessing

bool PacketProcessing::processPacket() {
    uint32_t headerCode = serialCom.nextPacketHeaderCode();

    switch (headerCode) {
    case 0:
        return false;
    case ACCELEROMETER_DATA_HEADER_CODE:
        return processAccelerometerPacket();
    case GYROSCOPE_DATA_HEADER_CODE:
        return processGyroscopePacket();
    case ALTIMETER_DATA_HEADER_CODE:
        return processAltimeterPacket();
    case GPS_DATA_HEADER_CODE:
        return processGpsPacket();
    case MAGNETOMETER_DATA_HEADER_CODE:
        return processMagnetometerPacket();
    case PRESSURE_SENSOR_DATA_HEADER_CODE:
        return processPressureSensorPacket();
    case ROCKET_DATA_HEADER_CODE:
        return processRocketPacket();
    case TEMPERATURE_SENSOR_DATA_HEADER_CODE:
        return processTemperatureSensorPacket();
    case VALVE_DATA_HEADER_CODE:
        return processValvePacket();
    }

    // Unknown packet header code
    return false;
}

bool PacketProcessing::processAccelerometerPacket() {
    AccelerometerPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(AccelerometerPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid AccelerometerPacket size, ignoring packet.");
        return false;
    }
    // TODO - Change altitude with accelerometer when sirius-headers-common fixed
    float timeStamp = packet.fields.rawData[0].members.timeStamp_ms;
    float rawAltitude = packet.fields.rawData[0].members.data.rawAltitude;
    PlotDataCenter::AccelerometerPlotData.addData(timeStamp, rawAltitude);
    return true;
}

bool PacketProcessing::processGyroscopePacket() {
    GyroscopePacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(GyroscopePacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid GyroscopePacket size, ignoring packet.");
        return false;
    }
    float timeStamp = packet.fields.rawData[0].members.timeStamp_ms;
    float rawX = packet.fields.rawData[0].members.data.rawX;
    float rawY = packet.fields.rawData[0].members.data.rawY;
    float rawZ = packet.fields.rawData[0].members.data.rawZ;
    PlotDataCenter::GyroscopePlotData.addData(timeStamp, rawX);
    return true;
}

bool PacketProcessing::processAltimeterPacket() {
    AccelerometerPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(AccelerometerPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid AccelerometerPacket size, ignoring packet.");
        return false;
    }
    float timeStamp = packet.fields.rawData[0].members.timeStamp_ms;
    float rawAltitude = packet.fields.rawData[0].members.data.rawAltitude;
    PlotDataCenter::AltimeterPlotData.addData(timeStamp, rawAltitude);
    return true;
}

bool PacketProcessing::processGpsPacket() {
    GpsPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(GpsPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid GpsPacket size, ignoring packet.");
        return false;
    }
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawLongitude = packet.fields.rawData.members.data.rawLongitude;
    float rawLatitude = packet.fields.rawData.members.data.rawLatitude;
    PlotDataCenter::GpsPlotData.addData(timeStamp, rawLongitude);
    return true;
}

bool PacketProcessing::processMagnetometerPacket() {
    MagnetometerPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(MagnetometerPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid MagnetometerPacket size, ignoring packet.");
        return false;
    }
    // Do something with packet...
    return true;
}

bool PacketProcessing::processPressureSensorPacket() {
    PressureSensorPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(PressureSensorPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid PressureSensorPacket size, ignoring packet.");
        return false;
    }
    // Do something with packet...
    return true;
}

bool PacketProcessing::processRocketPacket() {
    RocketPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(RocketPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid RocketPacket size, ignoring packet.");
        return false;
    }
    // Do something with packet...
    return true;
}

bool PacketProcessing::processTemperatureSensorPacket() {
    TemperatureSensorPacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(TemperatureSensorPacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid TemperatureSensorPacket size, ignoring packet.");
        return false;
    }
    // Do something with packet...
    return true;
}

bool PacketProcessing::processValvePacket() {
    ValvePacket packet;
    size_t packetSize = serialCom.getPacket(packet.data);
    if (packetSize != sizeof(ValvePacket)) {
        GCS_LOG_WARN("SerialComImpl: Invalid ValvePacket size, ignoring packet.");
        return false;
    }
    // Do something with packet...
    return true;
}
