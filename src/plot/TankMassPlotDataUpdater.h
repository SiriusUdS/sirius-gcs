#ifndef TANKMASSPLOTDATAUPDATER_H
#define TANKMASSPLOTDATAUPDATER_H

#include "PlotDataUpdateListener.h"

#include <map>
#include <mutex>
#include <vector>

class TankMassPlotDataUpdater : public PlotDataUpdateListener {
public:
    void onSubscribe(const PlotData* plotData) override;
    void onAddData(const PlotData* plotData, float x, float y) override;

private:
    struct PlotDataUpdate {
        float x{};
        float y{};
        bool wasUpdated{};
        std::chrono::steady_clock::time_point lastUpdateTime{};
    };

    std::map<const PlotData*, PlotDataUpdate> plotDataUpdateMap;
    std::mutex mtx;
};

#endif // TANKMASSPLOTDATAUPDATER_H
