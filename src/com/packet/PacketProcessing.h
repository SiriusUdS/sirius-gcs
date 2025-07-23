#ifndef PACKETPROCESSING_H

#include "CRC.h"
#include "Logging.h"

#include <stdint.h>
#include <typeinfo>

namespace PacketProcessing {
void processIncomingPackets();
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
