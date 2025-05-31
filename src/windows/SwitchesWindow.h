#ifndef SWITCHESWINDOW_H
#define SWITCHESWINDOW_H

struct SwitchData;

namespace SwitchesWindow {
void render();
void showSwitchState(const SwitchData* data);
} // namespace SwitchesWindow

#endif // SWITCHESWINDOW_H
