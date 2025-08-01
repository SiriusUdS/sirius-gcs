#include "ValvesWindow.h"

#include "FontConfig.h"
#include "GSDataCenter.h"
#include "OnOff.h"
#include "UIConfig.h"
#include "ValveData.h"

#include <imgui.h>
#include <string>

namespace ValvesWindow {
void renderValveStateRow(const char* label, ValveData& data);
} // namespace ValvesWindow

void ValvesWindow::render() {
    if (ImGui::BeginTable("ValveStateTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Valve");
        ImGui::TableSetupColumn("Is idle");
        ImGui::TableSetupColumn("Closed switch");
        ImGui::TableSetupColumn("Opened switch");
        ImGui::TableSetupColumn("% Open");
        ImGui::TableHeadersRow();

        renderValveStateRow("NOS", GSDataCenter::nosValveData);
        renderValveStateRow("IPA", GSDataCenter::ipaValveData);
        renderValveStateRow("Dump", GSDataCenter::dumpValveData);
        renderValveStateRow("Fill", GSDataCenter::fillValveData);

        ImGui::EndTable();
    }
}

void ValvesWindow::renderValveStateRow(const char* label, ValveData& data) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::Text(data.isIdle ? "Yes" : "No");

    ImGui::TableSetColumnIndex(2);
    ImGui::Text(data.closedSwitchHigh ? "On" : "Off");

    ImGui::TableSetColumnIndex(3);
    ImGui::Text(data.openedSwitchHigh ? "On" : "Off");

    ImGui::TableSetColumnIndex(4);
    ImGui::Text("%d", data.positionOpened_pct);
}
