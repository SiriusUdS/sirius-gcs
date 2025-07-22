#include "ValvesHeatPadsWindow.h"

#include "FontConfig.h"
#include "GSDataCenter.h"
#include "OnOff.h"
#include "UIConfig.h"
#include "ValveData.h"

#include <imgui.h>
#include <string>

namespace ValvesHeatPadsWindow {
void renderValveOrHeatPadState(const char* name, ValveData& data);
void renderValveOrHeatPadStateRow(const char* label, bool isOn);
} // namespace ValvesHeatPadsWindow

void ValvesHeatPadsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        renderValveOrHeatPadState("NOS Valve", GSDataCenter::nosValveData);
        renderValveOrHeatPadState("IPA Valve", GSDataCenter::ipaValveData);
        renderValveOrHeatPadState("Fill Valve", GSDataCenter::fillValveData);
        renderValveOrHeatPadState("Dump Valve", GSDataCenter::dumpValveData);
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        ImGui::Text("TODO");
    }
}

void ValvesHeatPadsWindow::renderValveOrHeatPadState(const char* name, ValveData& data) {
    ImGui::PushFont(FontConfig::boldDefaultFont);
    ImGui::Text(name);
    ImGui::PopFont();

    if (ImGui::BeginTable("ValveStateTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Signal");
        ImGui::TableSetupColumn("State");
        ImGui::TableHeadersRow();

        renderValveOrHeatPadStateRow("Is idle", data.isIdle);
        renderValveOrHeatPadStateRow("Closed switch high", data.closedSwitchHigh);
        renderValveOrHeatPadStateRow("Opened switch high", data.openedSwitchHigh);

        ImGui::EndTable();
    }
}

void ValvesHeatPadsWindow::renderValveOrHeatPadStateRow(const char* label, bool isOn) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::Text(isOn ? "On" : "Off");
}
