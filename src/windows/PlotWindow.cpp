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

/**
 * @brief Constructs a plot window
 * @param name Name of the plot window
 * @param xLabel Text to display along the X axis
 * @param yLabel Text to display along the Y axis
 * @param plotData The plot data object to render the data from on the plot
 */
PlotWindow::PlotWindow(const char* name, const char* xLabel, const char* yLabel, std::vector<PlotData*> plotData)
    : name(name), xLabel(xLabel), yLabel(yLabel), plotData(plotData) {
    autofitIniId = std::string(name) + std::string(Constants::GCS_INI_BASE_PLOT_WINDOW_AUTO_FIT);
    Utils::convertStringToIniId(autofitIniId);
}

/**
 * @brief Renders the plot window with ImGui
 */
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

/**
 * @brief Loads the state of the window from an ini file
 * @param ini The struct of the ini file
 */
void PlotWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(autofitIniId)) {
            autofit = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(autofitIniId));
        }
    }
}

/**
 * @brief Saves the state of the window to an ini file
 * @param ini The struct of the ini file
 */
void PlotWindow::saveState(mINI::INIStructure& ini) {
    ini[Constants::GCS_INI_SECTION].set(autofitIniId, std::to_string(autofit));
}

/**
 * @brief Generates a window id to uniquely identify different plot windows
 * @returns The generated window id
 */
std::string PlotWindow::getWindowId() {
    return std::string(Constants::GCS_BASE_PLOT_WINDOW_ID) + name;
}
