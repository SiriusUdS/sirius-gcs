#ifndef PACKETCIRCULARBUFFER_H
#define PACKETCIRCULARBUFFER_H

#include "CircularBuffer.hpp"
#include "SerialConfig.h"

using PacketCircularBuffer = CircularBuffer<SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE>;

#endif // PACKETCIRCULARBUFFER_H
