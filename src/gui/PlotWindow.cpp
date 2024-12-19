#include "PlotWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "PlotColors.h"

#include <imgui.h>
#include <implot.h>
#include <ini.h>

PlotWindow::PlotWindow() : Window("Plot"), flags(ImPlotFlags_None) {
    PlotData d1;
    d1.addData(0.0f, 0.0f);
    d1.addData(1.0f, 10.0f);
    d1.addData(2.0f, 7.5f);
    d1.addData(3.0f, 12.5f);
    d1.addData(4.0f, 15.0f);
    d1.setName("Data 1");
    d1.setColor(PlotColors::YELLOW);

    PlotData d2;
    d2.addData(0.0f, 0.0f);
    d2.addData(1.0f, 5.0f);
    d2.addData(2.0f, 12.5f);
    d2.addData(3.0f, 5.0f);
    d2.addData(4.0f, 10.0f);
    d2.setName("Data 2");
    d2.setColor(PlotColors::PURPLE);

    data.push_back(d1);
    data.push_back(d2);
}

void PlotWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT)) {
            autofit = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT));
        }
    }
}

void PlotWindow::saveState(mINI::INIStructure& ini) const {
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_PLOT_WINDOW_AUTO_FIT, std::to_string(autofit));
}

void PlotWindow::renderContent() {
    static float x = 5.0f;
    static float y = 2.5f;

    ImGui::Checkbox("Auto-fit", &autofit);
    if (autofit) {
        flags = ImPlotFlags_NoInputs;
        ImPlot::SetNextAxesToFit();
    } else {
        flags = ImPlotFlags_None;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add test data")) {
        for (auto& d : data) {
            d.addData(x, y++);
        }
        x++;
    }
    if (ImPlot::BeginPlot("Line Plot", ImGui::GetContentRegionAvail(), flags)) {
        for (const auto& plotData : data) {
            plotData.plot();
        }
        ImPlot::EndPlot();
    }
}
