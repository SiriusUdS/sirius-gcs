#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "RichMapPlot.h"
#include "RichMarkStorage.h"

class MapWindow {
public:
    MapWindow();
    void draw();

private:
    enum MapView { MAP_VIEW = 0, SATELLITE_VIEW = 1 };

    std::shared_ptr<RichMapPlot> _mapPlot;
    std::shared_ptr<MarkStorage> _storage;

    int _mapView{0};
    int _prevMapView{0};
};

#endif // MAPWINDOW_H
