#include "PlotWindow.h"

#include "IniConfig.h"
#include "SensorPlotData.h"
#include "Utils.h"

#include <imgui.h>
#include <implot.h>
#include <ini.h>
#include <iomanip>

/**
 * @brief Constructs a plot window
 * @param name Name of the plot window
 * @param xLabel Text to display along the X axis
 * @param yLabel Text to display along the Y axis
 * @param plotData The plot data object to render the data from on the plot
 */
PlotWindow::PlotWindow(const char* name, const char* xLabel, const char* yLabel, std::vector<SensorPlotData*> plotData)
    : name(name), xLabel(xLabel), yLabel(yLabel), plotData(plotData) {
    autofitIniId = std::string(name) + "_plot_window_auto_fit";
    showCompressedDataIniId = std::string(name) + "_plot_window_show_compressed_data";
    showAvgValuesId = std::string(name) + "_plot_window_show_avg_values";
    dataTypeIniId = std::string(name) + "_plot_window_data_type";
    Utils::convertStringToIniId(autofitIniId);
    Utils::convertStringToIniId(showCompressedDataIniId);
    Utils::convertStringToIniId(showAvgValuesId);
    Utils::convertStringToIniId(dataTypeIniId);
}

/**
 * @brief Renders the plot window with ImGui
 */
void PlotWindow::render() {
    ImGui::Checkbox("Auto-fit", &autofit);
    ImGui::SameLine();
    ImGui::Checkbox("Show compressed data", &showCompressedData);
    ImGui::SameLine();
    ImGui::Checkbox("Show avg. values", &showAvgValues);
    ImGui::SameLine();
    ImGui::RadioButton("Value", &dataType, VALUE);
    ImGui::SameLine();
    ImGui::RadioButton("ADC", &dataType, ADC);

    if (autofit) {
        flags = ImPlotFlags_NoInputs;
        ImPlot::SetNextAxesToFit();
    } else {
        flags = ImPlotFlags_None;
    }

    if (ImPlot::BeginPlot(name.c_str(), ImGui::GetContentRegionAvail(), flags)) {
        const char* actualYLabel = dataType == ADC ? "ADC Value" : yLabel.c_str();
        ImPlot::SetupAxes(xLabel.c_str(), actualYLabel);

        float recentAvgValue = 0.f;
        constexpr size_t recentAvgValueDurationMs = 2000;

        for (size_t i = 0; i < plotData.size(); i++) {
            const SensorPlotData* data = plotData[i];
            if (dataType == VALUE) {
                data->plotValue(showCompressedData);
                if (showAvgValues) {
                    showAvgRecentValue(data->getName(), data->averageRecentValue(recentAvgValueDurationMs), i);
                }
            } else {
                data->plotAdc(showCompressedData);
                if (showAvgValues) {
                    showAvgRecentValue(data->getName(), data->averageRecentAdc(recentAvgValueDurationMs), i);
                }
            }
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
        if (ini.get(IniConfig::GCS_SECTION).has(showAvgValuesId)) {
            showAvgValues = std::stoi(ini.get(IniConfig::GCS_SECTION).get(showAvgValuesId));
        }
        if (ini.get(IniConfig::GCS_SECTION).has(dataTypeIniId)) {
            dataType = std::stoi(ini.get(IniConfig::GCS_SECTION).get(dataTypeIniId));
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
    ini[IniConfig::GCS_SECTION].set(showAvgValuesId, std::to_string(showAvgValues));
    ini[IniConfig::GCS_SECTION].set(dataTypeIniId, std::to_string(dataType));
}

/**
 * @brief Generates a window id to uniquely identify different plot windows
 * @returns The generatsed window id
 */
std::string PlotWindow::getWindowId() {
    return "Plot - " + name;
}

void PlotWindow::showAvgRecentValue(const char* name, float value, size_t idx) {
    std::ostringstream oss;
    oss << name << " Avg. Value: " << std::fixed << std::setprecision(1) << value;
    const std::string avgValueStr = oss.str();
    const char* avgValueText = avgValueStr.c_str();

    ImDrawList* drawList = ImPlot::GetPlotDrawList();
    const ImVec2 plotPos = ImPlot::GetPlotPos();
    const ImVec2 plotSize = ImPlot::GetPlotSize();
    const ImVec2 textSize = ImGui::CalcTextSize(avgValueText);
    const ImVec2 textPos = {plotPos.x + plotSize.x - textSize.x - 10.f, plotPos.y + (textSize.y + 5.f) * idx + 15.f};
    const ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
    drawList->AddText(textPos, textColor, avgValueText);
}
