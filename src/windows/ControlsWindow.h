#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H

#include "SerialCom.h"

#include <chrono>

enum class ValveState { OPEN, CLOSED, OPENING, CLOSING };

namespace ControlsWindow {
void init();
void render();
void shutdown();

extern ValveState valveState1;
extern std::chrono::time_point<std::chrono::system_clock> valveTimer1;
extern ValveState valveState2;
extern std::chrono::time_point<std::chrono::system_clock> valveTimer2;
extern SerialCom serialCom;
} // namespace ControlsWindow

#endif // CONTROLSWINDOW_H
