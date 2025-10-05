#include "RecentPlotData.h"

#include "PlotData.h"

#include <implot.h>

RecentPlotData::RecentPlotData(const PlotData& plotData, size_t windowX) : plotData(plotData), windowX(windowX), start(0) {
}

void RecentPlotData::plot(bool showCompressedData) {
    PlotData::LockedView view = plotData.makeLockedView();
    const PlotRawData& data = showCompressedData ? view.getCompressedData() : view.getData();
    const PlotStyle& style = view.getStyle();

    if (data.size() == 0) {
        return;
    }

    const float minX = data.lastX() - windowX;

    if (start >= data.size()) {
        start = 0;
    }

    while (start > 0 && data.getXAt(start - 1) > minX) {
        start--;
    }

    while (start + 1 < data.size() && data.getXAt(start) < minX) {
        start++;
    }

    const size_t visibleCount = data.size() - start;
    if (visibleCount) {
        ImPlot::SetNextLineStyle(style.color, style.weight);
        ImPlot::PlotLine(style.name, data.getRawX() + start, data.getRawY() + start, static_cast<int>(visibleCount));
    }
}
