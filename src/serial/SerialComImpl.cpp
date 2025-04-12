#include "SerialComImpl.h"

#include "TemperatureSensor/TemperatureSensorPacket.h"
#include "WordFormatter.h"

namespace SerialComImpl {
SerialCom serialCom;
uint8_t buf[256] = {0};
} // namespace SerialComImpl

bool SerialComImpl::addTempDataToPlot(PlotData& data) {
    size_t size = serialCom.getPacket(buf);

    if (size != sizeof(TemperatureSensorPacket)) {
        return false;
    }

    TemperatureSensorPacket packet;
    for (size_t i = 0; i < sizeof(TemperatureSensorPacket); i++) {
        packet.data[i] = buf[i];
    }

    TelecommunicationHeader* header = &packet.fields.header;
    // header->values[0] = WordFormatter::flipu32(header->values[0]);

    TemperatureSensorPacketDataMembers* members = &packet.fields.rawData.members;
    // members->data.rawTemperature = WordFormatter::flipu32(members->data.rawTemperature);
    // members->errorStatus.value = WordFormatter::flipu16(members->errorStatus.value);
    // members->status.value = WordFormatter::flipu16(members->status.value);
    // members->timeStamp_ms = WordFormatter::flipu32(members->timeStamp_ms);

    data.addData((float) members->timeStamp_ms, (float) members->data.rawTemperature);

    return true;
}
