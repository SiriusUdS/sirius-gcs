#include "TankMassCalculatorWindow.h"

#include <CoolPropLib.h>
#include <cmath>
#include <imgui.h>

namespace TankMassCalculatorWindow {
double tankTemperature_C{};
double tankPressure_psi{};
} // namespace TankMassCalculatorWindow

void TankMassCalculatorWindow::render() {
    constexpr double TANK_VOLUME_M3 = 0.060516;

    ImGui::Text("Tank Volume (m^3): %f", TANK_VOLUME_M3);
    ImGui::InputDouble("Temperature (C)", &tankTemperature_C);
    ImGui::InputDouble("Pressure (psi)", &tankPressure_psi);

    const double tankTemperature_K = tankTemperature_C + 273.5;
    const double tankPressure_Pa = tankPressure_psi * 6894.76;
    const double rho = PropsSI("D", "T", tankTemperature_K, "P", tankPressure_Pa, "NitrousOxide");

    ImGui::Text("Tank Mass (kg): ");
    ImGui::SameLine();

    if (std::isnan(rho) || rho <= 0) {
        ImGui::Text("Invalid");
    } else {
        ImGui::Text("%f", rho * TANK_VOLUME_M3);
    }
}
