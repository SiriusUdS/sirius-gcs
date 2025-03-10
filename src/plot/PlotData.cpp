#include "PlotData.h"

#include "PlotColors.h"

#include <implot.h>

PlotData::PlotData() : name{"Unnamed"}, color{PlotColors::BLUE}, weight{2} {
    vx.reserve(BASE_DATA_VECTOR_CAPACITY);
    vy.reserve(BASE_DATA_VECTOR_CAPACITY);
}

/**
 * @brief Adds a single data point to the plot data.
 */
void PlotData::addData(float x, float y) {
    vx.push_back(x);
    vy.push_back(y);
}

/**
 * @brief Sets the name of the plot line (shown in the plot's legends).
 */
void PlotData::setName(const char* n) {
    name = n;
}

/**
 * @brief Sets the color of the plot line.
 */
void PlotData::setColor(ImVec4 c) {
    color = c;
}

/**
 * @brief Display the plot line. This should be called after a "ImPlot::BeginPlot" call.
 */
void PlotData::plot() const {
    ImPlot::SetNextLineStyle(color, weight);
    ImPlot::PlotLine(name, vx.data(), vy.data(), (int) vx.size());
}
