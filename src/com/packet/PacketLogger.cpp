#include "PacketLogger.h"

#include "Logging.h"

namespace PacketLogger {
const char* onOff(bool b);
} // namespace PacketLogger

void PacketLogger::logGsControl(GSControlStatus* packet) {
    // TODO LATER
    // GCS_DATA_LOG(GS_CONTROL_PACKET_LOG_TEMPLATE);
}

const char* PacketLogger::onOff(bool b) {
    return b ? "On" : "Off";
}
