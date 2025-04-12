#ifndef SERIALCOMIMPL_H
#define SERIALCOMIMPL_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

namespace SerialComImpl {
bool addTempDataToPlot(PlotData& data);

extern SerialCom serialCom;
extern uint8_t buf[];
} // namespace SerialComImpl

#endif // SERIALCOMIMPL_H
