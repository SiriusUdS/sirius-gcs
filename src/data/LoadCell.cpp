#include "LoadCell.h"

#include "Constants.h"
#include "Logging.h"

float LoadCell::convertRawToForce(float adcValue) {
    if (adcValue < Constants::ADC_MIN_LOADCELL || adcValue > Constants::ADC_MAX_LOADCELL) {
        GCS_LOG_ERROR("Load cell ADC value out of range!");
        return -1;
    }

    // TODO - Make sure this is correct formula (volts to lb)
    // float force = adcValue * Constants::LOADCELL_SCALE_FACTOR;
    float force = 200 * ((adcValue * 3.3f / 4096.f) / 209.f) / 0.015; // / 209.f;

    GCS_LOG_INFO("Load Cell Force: {} N", force);
    return force;
}
