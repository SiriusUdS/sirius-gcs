#include "TankMassPlotDataUpdater.h"

#include "GSDataCenter.h"
#include "Logging.h"
#include "SensorPlotData.h"
#include "TankMass.h"

void TankMassPlotDataUpdater::onSubscribe(const PlotData* plotData) {
    std::lock_guard<std::mutex> lock(mtx);
    plotDataUpdateMap.emplace(plotData, PlotDataUpdate());
}

void TankMassPlotDataUpdater::onAddData(const PlotData* plotData, float x, float y) {
    std::lock_guard<std::mutex> lock(mtx);

    auto updatedSourceIt = plotDataUpdateMap.find(plotData);
    if (updatedSourceIt == plotDataUpdateMap.end()) {
        GCS_APP_LOG_ERROR("TankMassPlotDataUpdater: Notifier plot data not found in tracked plot data map.");
        return;
    }
    PlotDataUpdate& updatedSource = updatedSourceIt->second;
    updatedSource.x = x;
    updatedSource.y = y;
    updatedSource.wasUpdated = true;
    updatedSource.lastUpdateTime = std::chrono::steady_clock::now();

    // Check if all tracked plot data where updated since last time a plot data point was computed
    // Also make sure the updates are recent enough
    constexpr auto MAX_TIME_DIFF = std::chrono::milliseconds(100000); // TODO: Make this lower in the future when not doing tests anymore
    bool allPlotDataUpdated = true;
    auto newest = std::chrono::steady_clock::time_point::min();
    auto oldest = std::chrono::steady_clock::time_point::max();

    for (const auto& [_, update] : plotDataUpdateMap) {
        if (!update.wasUpdated) {
            allPlotDataUpdated = false;
            break;
        }
        newest = std::max(newest, update.lastUpdateTime);
        oldest = std::min(oldest, update.lastUpdateTime);
    }

    if (!allPlotDataUpdated) {
        return;
    }

    if (newest - oldest > MAX_TIME_DIFF) {
        GCS_APP_LOG_DEBUG("TankMassPlotDataUpdater: Skipped update due to desynchronization ({}ms)",
                          std::chrono::duration_cast<std::chrono::milliseconds>(newest - oldest).count());
        return;
    }

    // TODO: Put indexes in constants and make sure they're the right ones
    const PlotData* tankTempPlotData = &GSDataCenter::Thermistor_Motor_PlotData[2].getValuePlotData();
    const PlotData* tankPressurePlotData = &GSDataCenter::PressureSensor_Motor_PlotData[0].getValuePlotData();

    auto tankTempPlotDataUpdateIt = plotDataUpdateMap.find(tankTempPlotData);
    if (tankTempPlotDataUpdateIt == plotDataUpdateMap.end()) {
        GCS_APP_LOG_ERROR("TankMassPlotDataUpdater: Tank temperature plot data not found in tracked plot data map.");
        return;
    }
    const PlotDataUpdate& tankTempPlotDataUpdate = tankTempPlotDataUpdateIt->second;

    auto tankPressurePlotDataUpdateIt = plotDataUpdateMap.find(tankPressurePlotData);
    if (tankPressurePlotDataUpdateIt == plotDataUpdateMap.end()) {
        GCS_APP_LOG_ERROR("TankMassPlotDataUpdater: Tank pressure plot data not found in tracked plot data map.");
        return;
    }
    const PlotDataUpdate& tankPressurePlotDataUpdate = tankPressurePlotDataUpdateIt->second;

    const double tankTemperature_C = static_cast<double>(tankTempPlotDataUpdate.y);
    const double tankPressure_psi = static_cast<double>(tankPressurePlotDataUpdate.y);
    const float tankMass = TankMass::getNOSTankMass_lb(tankTemperature_C, tankPressure_psi);

    // Get latest timestamp for new tank mass data point
    float latestX = 0.0f;
    for (const auto& [_, update] : plotDataUpdateMap) {
        latestX = std::max(latestX, update.x);
    }

    if (std::isnan(tankMass)) {
        GCS_APP_LOG_ERROR("TankMassPlotDataUpdater: Computed tank mass is \"NaN\".");
    } else if (std::isinf(tankMass)) {
        GCS_APP_LOG_ERROR("TankMassPlotDataUpdater: Computed tank mass is \"inf\".");
    } else {
        GSDataCenter::NOSTankMass_PlotData.addData(latestX, tankMass);
    }

    // Mark all plot data update as not recently updated
    for (auto& [_, update] : plotDataUpdateMap) {
        update.wasUpdated = false;
    }
}
