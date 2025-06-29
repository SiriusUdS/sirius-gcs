#include "LoadCell.h"

#include "Logging.h"
#include "PlotConfig.h"

float LoadCell::adcToForce(float adcValue) {
    constexpr float ADC_MIN_LOADCELL = 0;
    constexpr float ADC_MAX_LOADCELL = 4095;
    constexpr float LOADCELL_SCALE_FACTOR = 0.01f;

    // TODO: UNCOMMENT LATER
    // if (adcValue < ADC_MIN_LOADCELL || adcValue > ADC_MAX_LOADCELL) {
    //    GCS_LOG_DEBUG("LoadCell:: ADC value out of range.");
    //    return PlotConfig::INVALID_VALUE;
    //}

    // TODO - Make sure this is correct formula (volts to lb)
    // float force = adcValue * LOADCELL_SCALE_FACTOR;
    return 200 * ((adcValue * 3.3f / 4096.f) / 209.f) / 0.015f;
}
