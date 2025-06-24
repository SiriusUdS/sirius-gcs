#ifndef SENSORPLOTDATA_H
#define SENSORPLOTDATA_H

#include "PlotData.h"

class SensorPlotData {
public:
    SensorPlotData(const char* name, ImVec4 color);
    void addData(float adc, float value, float timestamp);
    void plotAdc(bool showCompressedData) const;
    void plotValue(bool showCompressedData) const;

private:
    PlotData adcPlotData;
    PlotData valuePlotData;
};

#endif // SENSORPLOTDATA_H
