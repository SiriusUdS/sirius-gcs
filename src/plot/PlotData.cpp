#include "PlotData.h"

#include "Constants.h"
#include "Logging.h"
#include "PlotColors.h"

#include <implot.h>

PlotData::PlotData(const char* n, ImVec4 c) : name{n}, color{c}, weight{2} {
}

/**
 * @brief Adds a single data point to the plot data.
 * @param x Where the data is placed on the X axis.
 * @param y Where the data is placed on the Y axis.
 */
void PlotData::addData(float x, float y) {
    std::lock_guard<std::mutex> lock(mtx);

    if (vx.size() && x < vx.back()) {
        GCS_LOG_WARN("PlotData: Received unordered data for plot data {}, clearing data.", name);
        vx.clear();
        vy.clear();
        return;
    }

    vx.push_back(x);
    vy.push_back(y);
    compressedVx.push_back(x);
    compressedVy.push_back(y);

    if (vx.size() > Constants::PLOT_MAX_DATA_SIZE_ORIGINAL) {
        dropOldData(Constants::PLOT_DATA_AMOUNT_TO_DROP_IF_MAX_REACHED);
    }

    if (compressedVx.size() > Constants::PLOT_MAX_DATA_SIZE_COMPRESSED) {
        compress(Constants::PLOT_TARGET_DATA_SIZE_COMPRESSED);
    }
}

/**
 * @brief Compresses the data down to a target amount of data points. It splices the data in segments (bins),
 *        then takes the average data point of each segment to generate the new compressed data.
 * @param targetSize The target size to compress the original data down to.
 */
void PlotData::compress(size_t targetSize) {
    if (vx.size() != vy.size()) {
        GCS_LOG_WARN("PlotData: Tried to compress plot data {}, but X and Y data sizes differ.", name);
        return;
    }

    const size_t dataSize = vx.size();
    const size_t compressedDataSize = compressedVx.size();

    if (dataSize <= targetSize || targetSize == 0) {
        GCS_LOG_DEBUG("PlotData: Compression unnecessary for plot data {}, current size {} smaller than target size {}.", name, dataSize, targetSize);
        return;
    }

    const size_t binSize = dataSize / targetSize;
    std::vector<float> newVx, newVy;

    for (size_t i = 0; i < dataSize; i += binSize) {
        float sumX = 0;
        float sumY = 0;
        size_t end = std::min(i + binSize, dataSize);
        size_t count = 0;

        for (size_t j = i; j < end; j++) {
            sumX += vx[j];
            sumY += vy[j];
            count++;
        }

        if (count > 0) {
            newVx.push_back(sumX / (float) count);
            newVy.push_back(sumY / (float) count);
        }
    }

    compressedVx = std::move(newVx);
    compressedVy = std::move(newVy);

    GCS_LOG_DEBUG("PlotData: Plot data {} successfully compressed from size {} to {}.", name, compressedDataSize, targetSize);
}

/**
 * @brief Drops a fixed amount of data points, starting from the oldest data.
 * @param amount Amount of data to drop.
 */
void PlotData::dropOldData(size_t amount) {
    size_t dataSize = vx.size();

    if (dataSize < amount) {
        GCS_LOG_DEBUG("PlotData: Dropping old data unnecessary for plot data {}, current size {} smaller than amount to drop {}.", name, dataSize,
                      amount);
        return;
    }

    vx.erase(vx.begin(), vx.begin() + amount);
    vy.erase(vy.begin(), vy.begin() + amount);

    compress(Constants::PLOT_TARGET_DATA_SIZE_COMPRESSED);

    GCS_LOG_DEBUG("PlotData: Plot data {} successfully dropped old data, went from size {} to {}.", name, dataSize, vx.size());
}

/**
 * @brief Display the plot line. This should be called after a "ImPlot::BeginPlot" call.
 */
void PlotData::plot() const {
    std::lock_guard<std::mutex> lock(mtx);
    ImPlot::SetNextLineStyle(color, weight);
    ImPlot::PlotLine(name, compressedVx.data(), compressedVy.data(), (int) compressedVx.size());
}
