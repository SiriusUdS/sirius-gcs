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

    if (ImGui::CollapsingHeader("Heat pads")) {
        if (ImGui::Button("NOS valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::NOS, 50);
        }
        if (ImGui::Button("IPA valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::IPA, 50);
        }
        if (ImGui::Button("Fill valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::FILL, 50);
        }
        if (ImGui::Button("Dump valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::DUMP, 50);
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
    bool sliderChanged;
    bool needToSynchronize;
    {
        std::lock_guard<std::mutex> lock(data.mtx);
        sliderChanged = ImGui::SliderInt(data.name, &data.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
        needToSynchronize = data.openedValue_perc != data.lastOpenedValue_perc;
    }

    if (!commandAvailable)
        return;

    if (sliderChanged || needToSynchronize) {
        std::lock_guard<std::mutex> lock(data.mtx);
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

    bool sliderEnabled = !(isDumpSelectedAndOff || isFillSelectedAndOff);

    if (data.wasSliderEnabled != sliderEnabled) {
        std::lock_guard<std::mutex> lock(data.mtx);
        if (!sliderEnabled) {
            data.lastOpenedValue_perc = 0;
            data.openedValue_perc = 0;
            CommandDispatch::valve(CommandCenter::valve1Command, 0);
        } else {
            data.lastOpenedValue_perc = data.openedValue_perc;
            CommandDispatch::valve(CommandCenter::valve1Command, data.openedValue_perc);
        }
        data.wasSliderEnabled = sliderEnabled;
    }

    ImGui::BeginDisabled(!sliderEnabled);
    bool sliderChanged;
    {
        std::lock_guard<std::mutex> lock(data.mtx);
        sliderChanged = ImGui::SliderInt(data.name, &data.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    }
    ImGui::EndDisabled();

    if (!sliderEnabled)
        return;

    bool needToSynchronize;
    {
        std::lock_guard<std::mutex> lock(data.mtx);
        needToSynchronize = data.openedValue_perc != data.lastOpenedValue_perc;
    }

    if (sliderChanged || needToSynchronize) {
        std::lock_guard<std::mutex> lock(data.mtx);
        data.lastOpenedValue_perc = data.openedValue_perc;
        CommandDispatch::valve(CommandCenter::valve1Command, data.openedValue_perc);
    }

    // Update wasSliderEnabled for next frame
    data.wasSliderEnabled = sliderEnabled;
}