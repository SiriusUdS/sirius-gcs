#include "MapWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "TileLoaderImpl.h"

MapWindow::MapWindow() {
    mapPlot = std::make_shared<RichMapPlot>();
    storage = std::make_shared<MarkStorage>();

    storage->addMark({46.14665264871996, -70.66861153239353}, "Tapis Venture");
    mapPlot->addItem(std::reinterpret_pointer_cast<IRichItem>(storage->markItems().back().ptr));
}

void MapWindow::render() {
    ImGui::Text("View type: ");
    ImGui::SameLine();
    ImGui::RadioButton("Map", &mapView, MAP_VIEW);
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

void MapWindow::loadState(const mINI::INIStructure& ini) {
    mapPlot->loadState(ini);
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW)) {
            mapView = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW));
        }
    }
}

void MapWindow::saveState(mINI::INIStructure& ini) const {
    mapPlot->saveState(ini);
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW, std::to_string(mapView));
}
