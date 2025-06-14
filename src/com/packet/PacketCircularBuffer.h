#ifndef PACKETCIRCULARBUFFER_H
#define PACKETCIRCULARBUFFER_H

#include "CircularBuffer.hpp"

constexpr size_t PACKET_CIRCULAR_BUFFER_SIZE = 10'000;

using PacketCircularBuffer = CircularBuffer<PACKET_CIRCULAR_BUFFER_SIZE>;

#endif // PACKETCIRCULARBUFFER_H
