#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <imgui.h>
#include <mutex>
#include <vector>

/**
 * @class PlotData
 * @brief Represents data of a single plot line to be shown on an ImPlot ImGui component
 */
class PlotData {
public:
    PlotData(const char* n, ImVec4 c);
    void addData(float x, float y);
    void compress(size_t targetSize);
    void dropOldData(size_t amount);
    void plot() const;

private:
    std::vector<float> vx;
    std::vector<float> vy;
    std::vector<float> compressedVx;
    std::vector<float> compressedVy;
    const char* name;
    ImVec4 color;
    float weight;
    mutable std::mutex mtx;
};

#endif // PLOTDATA_H
