#ifndef PLOTSTYLE_H
#define PLOTSTYLE_H

#include <imgui.h>

/**
 * @struct PlotStyle
 * @brief Style parameters for an ImPlot data line
 */
struct PlotStyle {
    const char* name; ///< Name of the plot data, shown in the plot's legend
    ImVec4 color;     ///< Color of the data line
    float weight;     ///< Thickness of the data line
};

#endif // PLOTSTYLE_H
