#include "LoadCell.h"

#include "Logging.h"

float LoadCell::convertRawToForce(float adcValue) {
    constexpr float ADC_MIN_LOADCELL = 0;
    constexpr float ADC_MAX_LOADCELL = 4095;
    constexpr float LOADCELL_SCALE_FACTOR = 0.01f;

    if (adcValue < ADC_MIN_LOADCELL || adcValue > ADC_MAX_LOADCELL) {
        // GCS_LOG_ERROR("Load cell ADC value out of range!");
        return -1;
    }

    // TODO - Make sure this is correct formula (volts to lb)
    // float force = adcValue * LOADCELL_SCALE_FACTOR;
    float force = 200 * ((adcValue * 3.3f / 4096.f) / 209.f) / 0.015f;

    // GCS_LOG_INFO("Load Cell Force: {} N", force);
    return force;
}
