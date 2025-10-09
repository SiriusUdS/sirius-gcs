#ifndef TANKMASSPLOTDATA_H
#define TANKMASSPLOTDATA_H

#include "PlotDataUpdateListener.h"

#include <chrono>
#include <map>
#include <vector>

class TankMassPlotData : public PlotDataUpdateListener {
public:
    void subscribeToPlotData(const std::vector<PlotData*> plotDataVec);
    void onAddData(const PlotData* plotData, float x, float y) override;
    void onClearData() override;

private:
    struct PlotDataUpdate {
        bool wasUpdated{};
        std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime{};
    };

    std::map<PlotData*, PlotDataUpdate> plotDataUpdateMap;
};

#endif // TANKMASSPLOTDATA_H
