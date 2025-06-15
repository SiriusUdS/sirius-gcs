#ifndef PRESSURETRANSDUCER_H
#define PRESSURETRANSDUCER_H

#include <stdint.h>

namespace PressureTransducer {
float adcToPressure(float adcValue, uint8_t sensorIndex);
} // namespace PressureTransducer

#endif // PRESSURETRANSDUCER_H
