#include "SwitchesWindow.h"

#include "GSDataCenter.h"
#include "OnOff.h"
#include "SwitchData.h"
#include "UIConfig.h"

#include <imgui.h>
#include <string>

void SwitchesWindow::render() {
    for (const SwitchData* switchData : GSDataCenter::SwitchDataVec) {
        const char* switchName = switchData->name;
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", switchName);
        ImGui::SameLine(260);
        OnOff(switchData->isOn, switchName);
    }
}
