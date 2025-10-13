#include "Params.h"

#include "IniConfig.h"
#include "RocketParams.h"

namespace Params {
const std::string INI_PARAMS_ROCKET_TANK_VOLUME_M3{"params_rocket_tank_volume_m3"};
}

void Params::loadParams(const mINI::INIStructure& ini) {
    if (ini.has(IniConfig::GCS_SECTION)) {
        if (ini.get(IniConfig::GCS_SECTION).has(INI_PARAMS_ROCKET_TANK_VOLUME_M3)) {
            RocketParams::NOSTankVolume_m3.currentValue = std::stof(ini.get(IniConfig::GCS_SECTION).get(RocketParams::NOSTankVolume_m3.iniKey));
        }
    }
}

void Params::saveParams(mINI::INIStructure& ini) {
    ini[IniConfig::GCS_SECTION].set(RocketParams::NOSTankVolume_m3.iniKey, std::to_string(RocketParams::NOSTankVolume_m3.currentValue));
}
