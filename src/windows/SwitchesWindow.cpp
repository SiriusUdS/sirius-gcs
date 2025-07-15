#include "SwitchesWindow.h"

#include "GSDataCenter.h"
#include "SwitchData.h"
#include "UIConfig.h"

#include <imgui.h>
#include <string>

void SwitchesWindow::render() {
    for (const SwitchData* switchData : GSDataCenter::SwitchDataVec) {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", switchData->name);
        ImGui::SameLine(260);
        showSwitchState(switchData);
    }
}

void SwitchesWindow::showSwitchState(const SwitchData* data) {
    std::string label = data->isOn ? "ON###" : "OFF###";
    label += data->name;
    ImVec4 color = data->isOn ? UIConfig::GREEN_BUTTON_COLOR : UIConfig::RED_BUTTON_COLOR;

    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

    ImGui::Button(label.c_str(), ImVec2(180, 0));

    ImGui::PopStyleColor(3);
}
