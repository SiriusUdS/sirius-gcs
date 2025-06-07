#ifndef PLOTDATACOMPRESSION_H
#define PLOTDATACOMPRESSION_H

class PlotRawData;

namespace PlotDataCompression {
void meanCompression(const PlotRawData& originalData, PlotRawData& compressedData, size_t targetCompressionSize, const char* name = "Unnamed");
}

#endif // PLOTDATACOMPRESSION_H
