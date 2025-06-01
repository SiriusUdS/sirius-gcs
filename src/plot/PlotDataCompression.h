#ifndef PLOTDATACOMPRESSION_H
#define PLOTDATACOMPRESSION_H

class PlotRawData;

namespace PlotDataCompression {
void compress(const PlotRawData& originalData, PlotRawData& compressedData, size_t targetCompressionSize, const char* name);
}

#endif // PLOTDATACOMPRESSION_H
