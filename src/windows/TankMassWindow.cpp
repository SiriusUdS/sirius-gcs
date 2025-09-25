#include "TankMassWindow.h"

#include <imgui.h>
#include <implot.h>

void TankMassWindow::render() {
    constexpr double TEMP_VALUE = 1.0;
    constexpr float TEMP_X[5] = {1, 2, 3, 4, 5};
    constexpr float TEMP_Y[5] = {0, 3, 1, 2, 5};

    if (ImGui::BeginTable("Tank Mass", 2)) {
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
            if (ImPlot::BeginPlot("Test 1")) {
                ImPlot::PlotLine("Test 1-1", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }
            ImGui::TableSetColumnIndex(1);
            if (ImPlot::BeginPlot("Test 2")) {
                ImPlot::PlotLine("Test 2-1", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImPlot::BeginPlot("Test 3")) {
                ImPlot::PlotLine("Test 3-1", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }
            ImGui::TableSetColumnIndex(1);
            if (ImPlot::BeginPlot("Test 4")) {
                ImPlot::PlotLine("Test 4-1", TEMP_X, TEMP_Y, 5);
                ImPlot::EndPlot();
            }

            ImGui::EndTable();
        }

        ImGui::EndTable();
    }
}
