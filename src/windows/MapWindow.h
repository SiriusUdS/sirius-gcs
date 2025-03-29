#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "RichMapPlot.h"
#include "RichMarkStorage.h"
#include "TileGrabber.h"
#include "TileSourceUrlConnTest.h"
#include "TileSourceUrlImpl.h"

#include <chrono>

namespace MapWindow {
enum MapView { MAP_VIEW = 0, SATELLITE_VIEW = 1 };

void init();
void loadState(const mINI::INIStructure& ini);
void saveState(mINI::INIStructure& ini);
void render();
void addMark(const GeoCoords& coords, const std::string& name);
std::string getFsPathFromMapView(int mapView);
void startTileProviderConnectivityTest();

extern int mapView;
extern int prevMapView;
extern bool sourceIsFs;
extern bool canFetchTilesFromUrl;
extern int downloadMinZ;
extern int downloadMaxZ;
extern size_t downloadTileCount;
extern size_t downloadTileTotal;
extern float downloadProgress;
extern std::chrono::seconds autoSourceSwitchDelay;
extern std::chrono::steady_clock::time_point lastAutoSourceSwitchTime;
extern std::shared_ptr<RichMapPlot> mapPlot;
extern std::shared_ptr<MarkStorage> storage;
extern std::shared_ptr<TileGrabber> mapTileGrabber;
extern std::shared_ptr<TileGrabber> satelliteTileGrabber;
extern std::shared_ptr<TileSourceUrlConnTest> urlConnectionTest;
} // namespace MapWindow

#endif // MAPWINDOW_H
