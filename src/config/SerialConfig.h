#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H

namespace SerialConfig {
constexpr size_t PACKET_CIRCULAR_BUFFER_SIZE = 10'000;
constexpr size_t SERIAL_TASK_LOOPS_PER_SECOND = 20;
} // namespace SerialConfig

#endif // SERIALCONFIG_H
