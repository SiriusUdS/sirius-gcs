#ifndef PLOTSTYLE_H
#define PLOTSTYLE_H

#include <imgui.h>

struct PlotStyle {
    const char* name;
    ImVec4 color;
    float weight;
};

#endif // PLOTSTYLE_H
