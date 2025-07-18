#include "PlotDataCompression.h"

#include "Logging.h"
#include "PlotRawData.h"

void PlotDataCompression::meanCompression(const PlotRawData& originalData, PlotRawData& compressedData, size_t targetCompressionSize, const char* name) {
    const size_t dataSize = originalData.size();
    const size_t compressedDataSize = compressedData.size();

    if (dataSize <= targetCompressionSize || targetCompressionSize == 0) {
        GCS_APP_LOG_DEBUG("PlotData: Compression unnecessary for plot data {}, current size {} smaller than target size {}.", name, dataSize,
                      targetCompressionSize);
        compressedData = originalData;
        return;
    }

    const double averageBinSize = dataSize / (double) targetCompressionSize;
    double binAccumulator = 0;
    size_t currentBinSize = 0;
    PlotRawData newData;

    for (size_t i = 0; i < dataSize; i += currentBinSize) {
        binAccumulator += averageBinSize;
        currentBinSize = (size_t) binAccumulator;
        binAccumulator -= currentBinSize;

        float sumX = 0;
        float sumY = 0;
        size_t end = std::min(i + currentBinSize, dataSize);
        size_t count = 0;

        for (size_t j = i; j < end; j++) {
            sumX += originalData.getXAt(j);
            sumY += originalData.getYAt(j);
            count++;
        }

        if (count > 0) {
            newData.add(sumX / (float) count, sumY / (float) count);
        }
    }

    compressedData = std::move(newData);

    GCS_APP_LOG_DEBUG("PlotData: Plot data {} successfully compressed from size {} to {}.", name, compressedDataSize, compressedData.size());
}
