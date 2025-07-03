#include "LoadCell.h"

#include "Logging.h"
#include "PlotConfig.h"

namespace LoadCell {
struct LoadCellParams {
    double capacity{};
    double additiveFactor{};
};

constexpr size_t LOAD_CELL_AMOUNT = 4;

// clang-format off
constexpr LoadCellParams LOAD_CELL_PARAMS_TABLE[LOAD_CELL_AMOUNT] = {
    {5000.0, 0.0},
    {5000.0, 0.0},
    {200.0,  0.0},
    {200.0,  286.5}
};
// clang-format on
} // namespace LoadCell

float LoadCell::adcToForce(float adcValue, size_t loadCellIndex) {
    const LoadCellParams& params = LOAD_CELL_PARAMS_TABLE[loadCellIndex];
    const double adjustedAdcValue = adcValue - params.additiveFactor;
    const double voltage = (adjustedAdcValue * 3.3) / 4096.0;

    return (float) (params.capacity * (voltage / 209) / 0.015);
}
