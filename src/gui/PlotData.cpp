#include "PlotData.h"

#include "PlotColors.h"

#include <implot.h>

PlotData::PlotData() : name{"Unnamed"}, color{PlotColors::BLUE}, weight{2} {
    vx.reserve(BASE_DATA_VECTOR_CAPACITY);
    vy.reserve(BASE_DATA_VECTOR_CAPACITY);
}

void PlotData::addData(float x, float y) {
    vx.push_back(x);
    vy.push_back(y);
}

void PlotData::setName(const char* n) {
    name = n;
}

void PlotData::setColor(ImVec4 c) {
    color = c;
}

void PlotData::plot() const {
    ImPlot::SetNextLineStyle(color, weight);
    ImPlot::PlotLine(name, vx.data(), vy.data(), vx.size());
}
