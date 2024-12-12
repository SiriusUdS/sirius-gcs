#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "RichMapPlot.h"
#include "RichMarkStorage.h"
#include "Window.h"

class MapWindow : public Window {
public:
    MapWindow();
    void renderContent();

private:
    enum MapView { MAP_VIEW = 0, SATELLITE_VIEW = 1 };

    std::shared_ptr<RichMapPlot> mapPlot;
    std::shared_ptr<MarkStorage> storage;

    int mapView{0};
    int prevMapView{0};
};

#endif // MAPWINDOW_H
