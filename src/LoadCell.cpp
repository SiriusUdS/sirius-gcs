#include "../Constants.h"
#include "LoadCell.h"
#include "logging/Logging.h"

float LoadCell::convertRawToForce(float adcValue) {
    if (adcValue < Constants::ADC_MIN_LOADCELL || adcValue > Constants::ADC_MAX_LOADCELL) {
        GCS_LOG_ERROR("Load cell ADC value out of range!");
        return -1;
    }

    float force = adcValue * Constants::LOADCELL_SCALE_FACTOR;

    GCS_LOG_INFO("Load Cell Force: {} N", force);
    return force;
}