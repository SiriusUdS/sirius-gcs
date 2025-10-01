#include "TankMassCalculatorWindow.h"

#include "FontAwesome.h"
#include "RocketParams.h"

#include <CoolPropLib.h>
#include <cmath>
#include <imgui.h>

namespace TankMassCalculatorWindow {
double tankTemperature_C{};
double tankPressure_psi{};
} // namespace TankMassCalculatorWindow

void TankMassCalculatorWindow::render() {
    ImGui::Text("Tank Volume (m^3): %f", RocketParams::tankVolume_m3);
    ImGui::SameLine();

    const ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
    ImGui::Button(ICON_FA_QUESTION);
    ImGui::PopStyleColor(2);

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("The tank volume can be modified applicationwide in the \"Rocket Parameters\" window.");
    }

    ImGui::InputDouble("Temperature (C)", &tankTemperature_C);
    ImGui::InputDouble("Pressure (psi)", &tankPressure_psi);

    const double tankTemperature_K = tankTemperature_C + 273.5;
    const double tankPressure_Pa = tankPressure_psi * 6894.76;
    const double rho = PropsSI("D", "T", tankTemperature_K, "P", tankPressure_Pa, "NitrousOxide");

    ImGui::Text("Tank Mass (kg): ");
    ImGui::SameLine();

    if (std::isnan(rho) || rho <= 0) {
        ImGui::Text("Invalid");
    } else if (std::isinf(rho)) {
        ImGui::Text("Infinity");
    } else {
        ImGui::Text("%f", rho * RocketParams::tankVolume_m3);
    }
}
