#include "RecentPlotData.h"

#include "PlotData.h"

#include <implot.h>

RecentPlotData::RecentPlotData(const PlotData& plotData, size_t windowX) : plotData(plotData), windowX(windowX), start(0) {
}

void RecentPlotData::setColor(ImVec4 color) {
    shouldOverrideColor = true;
    colorOverride = color;
}

void RecentPlotData::plot(bool showCompressedData) {
    PlotData::LockedView view = plotData.makeLockedView();
    const PlotRawData& data = showCompressedData ? view.getCompressedData() : view.getData();
    const PlotStyle& style = view.getStyle();
    const ImVec4& color = shouldOverrideColor ? colorOverride : style.color;
    const size_t size = data.size();

    if (start >= size) {
        start = 0;
    }

    if (size > 0) {
        const float minX = data.lastX() - windowX;

        while (start > 0 && data.getXAt(start - 1) > minX) {
            start--;
        }

        while (start < size && data.getXAt(start) < minX) {
            start++;
        }
    }

    const size_t visibleCount = size - start;
    ImPlot::SetNextLineStyle(color, style.weight);
    ImPlot::PlotLine(style.name, data.getRawX() + start, data.getRawY() + start, static_cast<int>(visibleCount));
}
