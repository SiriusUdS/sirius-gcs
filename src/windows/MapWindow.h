#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "RichMapPlot.h"
#include "RichMarkStorage.h"

#include <chrono>

namespace MapWindow {
enum MapView { MAP_VIEW = 0, SATELLITE_VIEW = 1 };

void render();
void loadState(const mINI::INIStructure& ini);
void saveState(mINI::INIStructure& ini);

extern int mapView;
extern int prevMapView;
extern bool sourceIsFs;
extern std::chrono::seconds autoSourceSwitchDelay;
extern std::chrono::steady_clock::time_point lastAutoSourceSwitchTime;
extern std::shared_ptr<RichMapPlot> mapPlot;
extern std::shared_ptr<MarkStorage> storage;
} // namespace MapWindow

#endif // MAPWINDOW_H
