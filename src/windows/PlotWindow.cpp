#include "PlotWindow.h"

#include "IniConfig.h"
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
    autofitIniId = std::string(name) + "_plot_window_auto_fit";
    showCompressedDataIniId = std::string(name) + "_plot_window_show_compressed_data";
    Utils::convertStringToIniId(autofitIniId);
    Utils::convertStringToIniId(showCompressedDataIniId);
}

/**
 * @brief Renders the plot window with ImGui
 */
void PlotWindow::render() {
    ImGui::Checkbox("Auto-fit", &autofit);
    ImGui::SameLine();
    ImGui::Checkbox("Show compressed data", &showCompressedData);

    if (autofit) {
        flags = ImPlotFlags_NoInputs;
        ImPlot::SetNextAxesToFit();
    } else {
        flags = ImPlotFlags_None;
    }

    if (ImPlot::BeginPlot(name.c_str(), ImGui::GetContentRegionAvail(), flags)) {
        ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str());
        for (const PlotData* data : plotData) {
            data->plot(showCompressedData);
        }
        ImPlot::EndPlot();
    }
}

/**
 * @brief Loads the state of the window from an ini file
 * @param ini The struct of the ini file
 */
void PlotWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(IniConfig::GCS_SECTION)) {
        if (ini.get(IniConfig::GCS_SECTION).has(autofitIniId)) {
            autofit = std::stoi(ini.get(IniConfig::GCS_SECTION).get(autofitIniId));
        }
        if (ini.get(IniConfig::GCS_SECTION).has(showCompressedDataIniId)) {
            showCompressedData = std::stoi(ini.get(IniConfig::GCS_SECTION).get(showCompressedDataIniId));
        }
    }
}

/**
 * @brief Saves the state of the window to an ini file
 * @param ini The struct of the ini file
 */
void PlotWindow::saveState(mINI::INIStructure& ini) {
    ini[IniConfig::GCS_SECTION].set(autofitIniId, std::to_string(autofit));
    ini[IniConfig::GCS_SECTION].set(showCompressedDataIniId, std::to_string(showCompressedData));
}

/**
 * @brief Generates a window id to uniquely identify different plot windows
 * @returns The generatsed window id
 */
std::string PlotWindow::getWindowId() {
    return "Plot - " + name;
}
