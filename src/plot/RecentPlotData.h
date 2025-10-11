#ifndef RECENTPLOTDATA_H
#define RECENTPLOTDATA_H

#include <imgui.h>

class PlotData;

class RecentPlotData {
public:
    RecentPlotData(const PlotData& plotData, size_t windowX);
    void setColor(ImVec4 color);
    void plot(bool showCompressedData);

private:
    const PlotData& plotData;
    size_t windowX;
    size_t start;
    bool shouldOverrideColor{};
    ImVec4 colorOverride{};
};

#endif // RECENTPLOTDATA_H
