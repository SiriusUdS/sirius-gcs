#ifndef PARAMS_H
#define PARAMS_H

#include <ini.h>

namespace Params {
void loadParams(const mINI::INIStructure& ini);
void saveParams(mINI::INIStructure& ini);
} // namespace Params

#endif // PARAMS_H
