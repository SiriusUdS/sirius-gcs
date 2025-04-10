#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <imgui.h>
#include <vector>

/**
 * @class PlotData
 * @brief Represents data of a single plot line to be shown on an ImPlot ImGui component
 */
class PlotData {
public:
    PlotData();
    void addData(float x, float y);
    void setName(const char* n);
    void setColor(ImVec4 c);
    void plot() const;

private:
    static const int BASE_DATA_VECTOR_CAPACITY = 1000;

    std::vector<float> vx;
    std::vector<float> vy;
    const char* name;
    ImVec4 color;
    float weight;
};

#endif // PLOTDATA_H
