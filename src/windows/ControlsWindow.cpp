#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "GSDataCenter.h"
#include "Logging.h"
#include "SwitchData.h"
#include "ValveStateData.h"
#include "Engine/EngineSensors.h"
#include "FillingStation/FillingStationSensors.h"

#include <imgui.h>

namespace ControlsWindow {

static void renderEngineValve(ValveData& data);
static void renderFillStationValve(ValveData& data);
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        for (size_t i = 0; i < GSDataCenter::VALVE_AMOUNT / 2; i++) {
            renderEngineValve(GSDataCenter::ValveEngineData[i]);
        }
        for (size_t i = 0; i < GSDataCenter::VALVE_AMOUNT / 2; i++) {
            renderFillStationValve(GSDataCenter::ValveFillStationData[i]);
        }
    }

    if (ImGui::CollapsingHeader("Commands")) {
        ImGui::BeginDisabled(!CommandCenter::command.available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
    }
}

void ControlsWindow::renderEngineValve(ValveData& data) {
    const bool commandAvailable = CommandCenter::valve1Command.available();
    const bool sliderChanged = ImGui::SliderInt(data.name, &data.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    const bool needToSynchronize = data.openedValue_perc != data.lastOpenedValue_perc;

    if (!commandAvailable)
        return;

    if (sliderChanged || needToSynchronize) {
        data.lastOpenedValue_perc = data.openedValue_perc;
        CommandDispatch::valve(CommandCenter::valve1Command, data.openedValue_perc);
    }
}

void ControlsWindow::renderFillStationValve(ValveData& data) {
    const bool commandAvailable = CommandCenter::valve1Command.available();

    if (!commandAvailable)
        return;

    bool isDumpSwitchOn = GSDataCenter::AllowDumpSwitchData.isOn;
    bool isFillSwitchOn = GSDataCenter::AllowFillSwitchData.isOn;

    bool isDumpSelectedAndOff = (!isDumpSwitchOn && data.id == FILLING_STATION_NOS_DUMP_VALVE_INDEX);
    bool isFillSelectedAndOff = (!isFillSwitchOn && data.id == FILLING_STATION_NOS_VALVE_INDEX);

    ImGui::BeginDisabled(
      isDumpSelectedAndOff ||
      isFillSelectedAndOff
    );
    const bool sliderChanged = ImGui::SliderInt(data.name, &data.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    ImGui::EndDisabled();

    if (isDumpSelectedAndOff || isFillSelectedAndOff)
      return;

    const bool needToSynchronize = data.openedValue_perc != data.lastOpenedValue_perc;

    if (sliderChanged || needToSynchronize) {
        data.lastOpenedValue_perc = data.openedValue_perc;
        CommandDispatch::valve(CommandCenter::valve1Command, data.openedValue_perc);
    }
}