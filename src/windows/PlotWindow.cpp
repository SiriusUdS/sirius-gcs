#include "PlotWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "PacketProcessing.h"
#include "PlotColors.h"
#include "PlotDataCenter.h"
#include "Utils.h"

#include <imgui.h>
#include <implot.h>
#include <ini.h>

PlotWindow::PlotWindow(const char* name, const char* xLabel, const char* yLabel, std::vector<PlotData*> plotData)
    : name(name), xLabel(xLabel), yLabel(yLabel), plotData(plotData) {
    autofitIniId = std::string(name) + std::string(Constants::GCS_INI_BASE_PLOT_WINDOW_AUTO_FIT);
    Utils::convertStringToIniId(autofitIniId);
}

void PlotWindow::render() {
    ImGui::Checkbox("Auto-fit", &autofit);
    if (autofit) {
        flags = ImPlotFlags_NoInputs;
        ImPlot::SetNextAxesToFit();
    } else {
        flags = ImPlotFlags_None;
    }

    if (ImPlot::BeginPlot(name.c_str(), xLabel.c_str(), yLabel.c_str(), ImGui::GetContentRegionAvail(), flags)) {
        for (const PlotData* data : plotData) {
            data->plot();
        }
        ImPlot::EndPlot();
    }
}

void PlotWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(autofitIniId)) {
            autofit = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(autofitIniId));
        }
    }
}

void PlotWindow::saveState(mINI::INIStructure& ini) {
    ini[Constants::GCS_INI_SECTION].set(autofitIniId, std::to_string(autofit));
}

std::string PlotWindow::getWindowId() {
    return std::string(Constants::GCS_BASE_PLOT_WINDOW_ID) + name;
}
