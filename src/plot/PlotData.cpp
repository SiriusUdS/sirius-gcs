#include "PlotData.h"

#include "Logging.h"
#include "PlotColors.h"
#include "PlotDataCompression.h"

#include <implot.h>

/**
 * @brief Constructs a plot data from a name and a color of the plot data line
 * @param n Name of the plot data
 * @param c Color of the plot data line
 */
PlotData::PlotData(const char* n, ImVec4 c) : style{n, c, 2} {
}

/**
 * @brief Adds a single data point to the plot data.
 * @param x Where the data is placed on the X axis.
 * @param y Where the data is placed on the Y axis.
 */
void PlotData::addData(float x, float y) {
    std::lock_guard<std::mutex> lock(mtx);

    if (data.size() && x < data.lastX()) {
        GCS_LOG_WARN("PlotData: Received unordered data for plot data {}, clearing data.", style.name);
        data.clear();
        compressedData.clear();
    }

    data.add(x, y);
    compressedData.add(x, y);

    if (data.size() > MAX_ORIGINAL_DATA_SIZE) {
        dropOldData(DATA_AMOUNT_TO_DROP_IF_MAX_REACHED);
    }

    if (compressedData.size() > MAX_COMPRESSED_DATA_SIZE) {
        PlotDataCompression::meanCompression(data, compressedData, TARGET_COMPRESSED_DATA_SIZE, style.name);
    }
}

/**
 * @brief Drops a fixed amount of data points, starting from the oldest data.
 * @param amount Amount of data to drop.
 */
void PlotData::dropOldData(size_t amount) {
    size_t oldDataSize = data.size();

    if (oldDataSize < amount) {
        GCS_LOG_DEBUG("PlotData: Dropping old data unnecessary for plot data {}, current size {} smaller than amount to drop {}.", style.name,
                      oldDataSize, amount);
        return;
    }

    data.eraseOld(amount);
    PlotDataCompression::meanCompression(data, compressedData, TARGET_COMPRESSED_DATA_SIZE, style.name);

    GCS_LOG_DEBUG("PlotData: Plot data {} successfully dropped old data, went from size {} to {}.", style.name, oldDataSize, data.size());
}

/**
 * @brief Display the plot line. This should be called after a "ImPlot::BeginPlot" call.
 * @param showCompressedData Plot compressed data to improve performances
 */
void PlotData::plot(bool showCompressedData) const {
    std::lock_guard<std::mutex> lock(mtx);
    ImPlot::SetNextLineStyle(style.color, style.weight);
    const PlotRawData& dataToPlot = showCompressedData ? compressedData : data;
    ImPlot::PlotLine(style.name, dataToPlot.getRawX(), dataToPlot.getRawY(), (int) dataToPlot.size());
}
