#include "LoadCell.h"

#include "Logging.h"
#include "PlotConfig.h"

namespace LoadCell {
struct LoadCellParams {
    double additiveFactor{};
    double functionRateOfChange{};
    double functionOffset{};
};

constexpr size_t LOAD_CELL_AMOUNT = 4;

// clang-format off
constexpr LoadCellParams LOAD_CELL_PARAMS_TABLE[LOAD_CELL_AMOUNT] = {
    {.additiveFactor=0.0, .functionRateOfChange=0.0231, .functionOffset=0.0838}, // Tank load cell
    {.additiveFactor=0.0, .functionRateOfChange=0.0,    .functionOffset=0.0}     // Combustion chamber load cell
};
// clang-format on
} // namespace LoadCell

float LoadCell::adcToForce(float adcValue, size_t loadCellIndex) {
    const LoadCellParams& params = LOAD_CELL_PARAMS_TABLE[loadCellIndex];
    return static_cast<float>(params.functionRateOfChange * adcValue) - static_cast<float>(params.functionOffset - params.additiveFactor);
}
