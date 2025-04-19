#include "PressureTransducer.h"

float voltageConverter_V(float adcValue) {
    float VoltageInput = adcValue / ADCRange_bit * voltageRange_V;
    return VoltageInput;
}

float pressureConverter_NAME1_PSI(float VoltageInput, uint8_t index) {
    return (PRESSURE_SENSOR_ARRAY_SLOPE[index] * VoltageInput) + PRESSURE_SENSOR_ARRAY_CONST[index];
}