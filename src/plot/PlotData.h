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
    void plot(bool showCompressedData) const;

private:
    static constexpr size_t MAX_ORIGINAL_DATA_SIZE = 100'000;
    static constexpr size_t DATA_AMOUNT_TO_DROP_IF_MAX_REACHED = 10'000;
    static constexpr size_t MAX_COMPRESSED_DATA_SIZE = 20'000;
    static constexpr size_t TARGET_COMPRESSED_DATA_SIZE = 10'000;

    PlotRawData data;
    PlotRawData compressedData;
    PlotStyle style;
    mutable std::mutex mtx;
};

#endif // PLOTDATA_H
