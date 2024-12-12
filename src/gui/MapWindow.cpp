#include "MapWindow.h"

#include "Logging.h"
#include "TileLoaderImpl.h"

MapWindow::MapWindow() : Window("Map") {
    mapPlot = std::make_shared<RichMapPlot>();
    storage = std::make_shared<MarkStorage>();

    storage->addMark({46.14665264871996, -70.66861153239353}, "Tapis Venture");
    mapPlot->addItem(std::reinterpret_pointer_cast<IRichItem>(storage->markItems().back().ptr));
}

void MapWindow::renderContent() {
    ImGui::Text("Type de vue: ");
    ImGui::SameLine();
    ImGui::RadioButton("Cartographique", &mapView, MAP_VIEW);
    ImGui::SameLine();
    ImGui::RadioButton("Satellite", &mapView, SATELLITE_VIEW);

    if (mapView != prevMapView) {
        if (mapView == MAP_VIEW) {
            mapPlot->setTileLoader(std::make_shared<TileLoaderOsmMap>());
        } else if (mapView == SATELLITE_VIEW) {
            mapPlot->setTileLoader(std::make_shared<TileLoaderArcMap>());
        }
        prevMapView = mapView;
    }

    mapPlot->paint();
}