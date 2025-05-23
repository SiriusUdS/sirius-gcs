#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H

#include "SerialCom.h"

#include <chrono>

namespace ControlsWindow {
enum class ValveState { OPEN, CLOSED, OPENING, CLOSING };

void render();
void renderValveState(const char* id, ValveState state);

extern ValveState valveState1;
extern ValveState valveState2;
} // namespace ControlsWindow

#endif // CONTROLSWINDOW_H
