#include "MapWindow.h"

#include "Logging.h"
#include "TileLoaderImpl.h"

MapWindow::MapWindow() : Window("Map") {
    _mapPlot = std::make_shared<RichMapPlot>();
    _storage = std::make_shared<MarkStorage>();

    _storage->addMark({46.14665264871996, -70.66861153239353}, "Tapis Venture");
    _mapPlot->addItem(std::reinterpret_pointer_cast<IRichItem>(_storage->markItems().back().ptr));
}

void MapWindow::renderContent() {
    ImGui::Text("Type de vue: ");
    ImGui::SameLine();
    ImGui::RadioButton("Cartographique", &_mapView, MAP_VIEW);
    ImGui::SameLine();
    ImGui::RadioButton("Satellite", &_mapView, SATELLITE_VIEW);

    if (_mapView != _prevMapView) {
        if (_mapView == MAP_VIEW) {
            _mapPlot->setTileLoader(std::make_shared<TileLoaderOsmMap>());
        } else if (_mapView == SATELLITE_VIEW) {
            _mapPlot->setTileLoader(std::make_shared<TileLoaderArcMap>());
        }
        _prevMapView = _mapView;
    }

    _mapPlot->paint();
}