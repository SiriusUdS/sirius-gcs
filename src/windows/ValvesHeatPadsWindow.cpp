#include "ValvesHeatPadsWindow.h"

#include "FontConfig.h"
#include "GSDataCenter.h"
#include "OnOff.h"
#include "UIConfig.h"
#include "ValveData.h"

#include <imgui.h>
#include <string>

namespace ValvesHeatPadsWindow {
void renderValveOrHeatPadStateRow(const char* label, ValveData& data);
} // namespace ValvesHeatPadsWindow

void ValvesHeatPadsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        if (ImGui::BeginTable("ValveStateTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Valve");
            ImGui::TableSetupColumn("Is idle");
            ImGui::TableSetupColumn("Closed switch");
            ImGui::TableSetupColumn("Opened switch");
            ImGui::TableHeadersRow();

            renderValveOrHeatPadStateRow("NOS", GSDataCenter::nosValveData);
            renderValveOrHeatPadStateRow("IPA", GSDataCenter::ipaValveData);
            renderValveOrHeatPadStateRow("Dump", GSDataCenter::dumpValveData);
            renderValveOrHeatPadStateRow("Fill", GSDataCenter::fillValveData);

            ImGui::EndTable();
        }
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        ImGui::Text("TODO");
        // if (ImGui::BeginTable("HeadPadStateTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        //     ImGui::TableSetupColumn("Heat Pad");
        //     ImGui::TableSetupColumn("Is idle");
        //     ImGui::TableSetupColumn("Closed switch");
        //     ImGui::TableSetupColumn("Opened switch");
        //     ImGui::TableHeadersRow();

        //    ImGui::EndTable();
        //}
    }
}

void ValvesHeatPadsWindow::renderValveOrHeatPadStateRow(const char* label, ValveData& data) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::Text(data.isIdle ? "Yes" : "No");

    ImGui::TableSetColumnIndex(2);
    ImGui::Text(data.closedSwitchHigh ? "On" : "Off");

    ImGui::TableSetColumnIndex(3);
    ImGui::Text(data.openedSwitchHigh ? "On" : "Off");
}
