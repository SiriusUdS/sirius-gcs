#include "NOSPhaseDiagramWindow.h"

#include "PlotData.h"

#include <imgui.h>
#include <implot.h>
#include <math.h>

namespace NOSPhaseDiagramWindow {
using PT_POINT = ImVec2; // Point type for Pressure-Temperature diagram (Pressure (psi), Temperature (C))

constexpr PT_POINT triplePoint{-90.82f, 12.74156526f};
constexpr PT_POINT criticalPoint{36.42f, 1050.7984099f};

ImVector<float> liquidPhaseTemp_C;
ImVector<float> liquidPhasePressure_psi;
ImVector<float> gasPhaseTemp_C;
ImVector<float> gasPhasePressure_psi;

float vaporPressureNOS_psi(float temperature_C);
} // namespace NOSPhaseDiagramWindow

void NOSPhaseDiagramWindow::init() {
    // Initialize the vaporization curve points
    liquidPhaseTemp_C.clear();
    liquidPhasePressure_psi.clear();
    gasPhaseTemp_C.clear();
    gasPhasePressure_psi.clear();

    liquidPhaseTemp_C.push_back(-144.15f);
    liquidPhasePressure_psi.push_back(criticalPoint.y);

    for (float temp_C = -144.15f; temp_C <= -86.15f; temp_C += .1f) {
        const float pressure_psi = vaporPressureNOS_psi(temp_C);
        liquidPhaseTemp_C.push_back(temp_C);
        liquidPhasePressure_psi.push_back(pressure_psi);
        gasPhaseTemp_C.push_back(temp_C);
        gasPhasePressure_psi.push_back(pressure_psi);
    }

    gasPhaseTemp_C.push_back(-86.15f);
    gasPhasePressure_psi.push_back(0.f);
}

void NOSPhaseDiagramWindow::render() {
    ImPlot::SetNextAxesToFit();
    if (ImPlot::BeginPlot("NOS Phase Diagram", ImGui::GetContentRegionAvail(), ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxes("Temperature (C)", "Pressure (psi)");
        ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);

        ImPlot::PushStyleColor(ImPlotCol_Fill, IM_COL32(40, 120, 255, 80));
        ImVector<float> liquidTop;
        liquidTop.resize(liquidPhaseTemp_C.size());
        for (int i = 0; i < liquidTop.size(); ++i) {
            liquidTop[i] = criticalPoint.y;
        }
        ImPlot::PlotShaded("Liquid Phase", liquidPhaseTemp_C.begin(), liquidPhasePressure_psi.begin(), liquidTop.begin(), liquidPhaseTemp_C.size());
        ImPlot::PopStyleColor();

        ImPlot::PushStyleColor(ImPlotCol_Fill, IM_COL32(255, 140, 40, 80));
        ImVector<float> gasBottom;
        gasBottom.resize(gasPhaseTemp_C.size());
        for (int i = 0; i < gasBottom.size(); ++i) {
            gasBottom[i] = 0.0f;
        }
        ImPlot::PlotShaded("Gas Phase", gasPhaseTemp_C.begin(), gasPhasePressure_psi.begin(), gasBottom.begin(), gasPhaseTemp_C.size());
        ImPlot::PopStyleColor();

        ImPlot::PlotLine("Vaporization Curve", liquidPhaseTemp_C.begin(), liquidPhasePressure_psi.begin(), liquidPhaseTemp_C.size());

        ImPlot::EndPlot();
    }
}

float NOSPhaseDiagramWindow::vaporPressureNOS_psi(float temperature_C) {
    // Antoine equation coefficients for nitrous oxide (-144.15 C to -86.15 C)
    static constexpr double A = 4.37799;
    static constexpr double B = 621.077;
    static constexpr double C = -44.659;
    const double temperature_K = temperature_C + 273.15;
    const float pressure_bar = static_cast<float>(pow(10, A - (B / (temperature_K + C))));
    return pressure_bar * 14.50377f;
}
