#ifndef SENSORPLOTDATA_H
#define SENSORPLOTDATA_H

#include "PlotData.h"

/**
 * @class SensorPlotData
 * @brief Plot data for sensor that includes ADC and value data.
 */
class SensorPlotData {
public:
    SensorPlotData(const char* name, ImVec4 color);
    void addData(float adc, float value, float timestamp);
    void plotAdc(bool showCompressedData) const;
    void plotValue(bool showCompressedData) const;
    float averageRecentAdc(size_t durationMs) const;
    float averageRecentValue(size_t durationMs) const;
    const char* getName() const;

private:
    PlotData adcPlotData;
    PlotData valuePlotData;
};

#endif // SENSORPLOTDATA_H
