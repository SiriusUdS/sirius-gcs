#ifndef RECENTPLOTDATA_H
#define RECENTPLOTDATA_H

class PlotData;

class RecentPlotData {
public:
    RecentPlotData(const PlotData& plotData, size_t windowX);
    void plot(bool showCompressedData);

private:
    const PlotData& plotData;
    size_t windowX;
    size_t start;
};

#endif // RECENTPLOTDATA_H
