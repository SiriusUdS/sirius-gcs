#include "PlotWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "PacketProcessing.h"
#include "PlotColors.h"
#include "PlotDataCenter.h"

#include <imgui.h>
#include <implot.h>
#include <ini.h>

namespace PlotWindow {
ImPlotFlags flags{};
bool autofit{};
} // namespace PlotWindow

void PlotWindow::render() {
    ImGui::Checkbox("Auto-fit", &autofit);
    if (autofit) {
        flags = ImPlotFlags_NoInputs;
        ImPlot::SetNextAxesToFit();
    } else {
        flags = ImPlotFlags_None;
    }

    if (ImPlot::BeginPlot("Line Plot", "Time (ms)", "Degrees (C)", ImGui::GetContentRegionAvail(), flags)) {
        PlotDataCenter::TemperatureSensorPlotData.plot();
        PlotDataCenter::AccelerometerXPlotData.plot();
        PlotDataCenter::AccelerometerYPlotData.plot();
        PlotDataCenter::AccelerometerZPlotData.plot();
        ImPlot::EndPlot();
    }
}

void PlotWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT)) {
            autofit = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT));
        }
    }
}

void PlotWindow::saveState(mINI::INIStructure& ini) {
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT, std::to_string(autofit));
}
