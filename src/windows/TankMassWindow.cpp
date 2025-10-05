#include "TankMassWindow.h"

#include "GSDataCenter.h"
#include "RecentPlotData.h"
#include "SensorPlotData.h"

#include <imgui.h>
#include <implot.h>

namespace TankMassWindow {
RecentPlotData recentEngineThrust{GSDataCenter::LoadCell_FillingStation_PlotData[0].getValuePlotData(),
                                  100}; // TODO: Add constants for index and window X
}

void TankMassWindow::render() {
    constexpr double TEMP_VALUE = 1.0;
    constexpr float TEMP_X[5] = {1, 2, 3, 4, 5};
    constexpr float TEMP_Y[5] = {0, 3, 1, 2, 5};

    constexpr int PLOT_ROWS = 2;

    const ImVec2 avail = ImGui::GetContentRegionAvail();
    const float paddingY = ImGui::GetStyle().CellPadding.y * 3.0f;
    const float plotRowHeight = (avail.y / PLOT_ROWS) - paddingY;

    if (ImGui::BeginTable("Tank Mass", PLOT_ROWS)) {
        ImGui::TableSetupColumn("Data", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Plots");
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Temp Tank: %f", TEMP_VALUE);
        ImGui::Text("PT Tank: %f", TEMP_VALUE);
        ImGui::Text("LC Tank: %f", TEMP_VALUE);
        ImGui::Text("Phase: %f", TEMP_VALUE);

        ImGui::TableSetColumnIndex(1);
        if (ImGui::BeginTable("Plots", 2)) {
            ImGui::TableSetupColumn("Col1");
            ImGui::TableSetupColumn("Col2");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImPlot::BeginPlot("Tank Pressure", ImVec2(-1, plotRowHeight))) {
                ImPlot::PlotLine("Tank Pressure", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }
            ImGui::TableSetColumnIndex(1);
            if (ImPlot::BeginPlot("Tank Temperature", ImVec2(-1, plotRowHeight))) {
                ImPlot::PlotLine("Tank Temperature", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImPlot::SetNextAxesToFit();
            if (ImPlot::BeginPlot("Engine Thrust", ImVec2(-1, plotRowHeight), ImPlotFlags_NoInputs)) {
                // ImPlot::PlotLine("Engine Thrust", TEMP_X, TEMP_Y, 5);
                recentEngineThrust.plot(false);
                ImPlot::EndPlot();
            }
            ImGui::TableSetColumnIndex(1);
            if (ImPlot::BeginPlot("Tank Mass", ImVec2(-1, plotRowHeight))) {
                ImPlot::PlotLine("Tank Mass", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }

            ImGui::EndTable();
        }

        ImGui::EndTable();
    }
}
