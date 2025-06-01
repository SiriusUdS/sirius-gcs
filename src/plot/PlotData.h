#ifndef PLOTDATA_H
#define PLOTDATA_H

#include "PlotRawData.h"
#include "PlotStyle.h"

#include <mutex>

/**
 * @class PlotData
 * @brief Represents data of a single plot line to be shown on an ImPlot ImGui component
 */
class PlotData {
public:
    PlotData(const char* n, ImVec4 c);
    void addData(float x, float y);
    void dropOldData(size_t amount);
    void plot() const;

private:
    PlotRawData data;
    PlotRawData compressedData;
    PlotStyle style;
    mutable std::mutex mtx;
};

#endif // PLOTDATA_H
