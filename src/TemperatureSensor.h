#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

namespace TemperatureSensor {
    float interpolateTemperature(float measuredResistance);
    float temperatureInfos(float adc_value);
} //namespace TemperatureSensor


#endif // TEMPERATURESENSOR_H