#include "MapWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "TileLoaderImpl.h"
#include "TileSaver.h"
#include "TileSourceUrlImpl.h"

namespace MapWindow {
int mapView{};
int prevMapView{};
bool sourceIsFs{};
bool canFetchTilesFromUrl{false};
std::chrono::seconds autoSourceSwitchDelay{3};
std::chrono::steady_clock::time_point lastAutoSourceSwitchTime{std::chrono::steady_clock::now()};
std::shared_ptr<RichMapPlot> mapPlot;
std::shared_ptr<MarkStorage> storage;
std::shared_ptr<TileGrabber> tileGrabber;
std::shared_ptr<TileSourceUrl> urlConnectionTest;
} // namespace MapWindow

void MapWindow::render() {
    if (ImGui::CollapsingHeader("Download Tiles")) {
        ImGui::Indent(20.0f);
        ImGui::Text("Min Lat %.6f, Min Lon %.6f", mapPlot->minLat(), mapPlot->minLon());
        ImGui::Text("Max Lat %.6f, Max Lon %.6f", mapPlot->maxLat(), mapPlot->maxLon());
        ImGui::Text("Zoom %d", mapPlot->zoom());
        if (ImGui::Button("Download")) {
            tileGrabber->grab(mapPlot->minLat(), mapPlot->maxLat(), mapPlot->minLon(), mapPlot->maxLon(), mapPlot->zoom(), mapPlot->zoom());
        }
        ImGui::Unindent(20.0f);
    }

    ImGui::TextUnformatted("View type: ");
    ImGui::SameLine();
    ImGui::RadioButton("Map", &mapView, MAP_VIEW);
    ImGui::SameLine();
    ImGui::RadioButton("Satellite", &mapView, SATELLITE_VIEW);

    //auto now = std::chrono::steady_clock::now();
    //bool autoSourceSwitchDelayElapsed = now - lastAutoSourceSwitchTime > autoSourceSwitchDelay;
    //bool shouldSwitchSource = mapPlot->failedToFetchTiles() && autoSourceSwitchDelayElapsed;
    //if (shouldSwitchSource) {
    //    sourceIsFs = !sourceIsFs;
    //    lastAutoSourceSwitchTime = now;
    //    GCS_LOG_WARN("Failed to fetch tiles, switching sources: {}", sourceIsFs);
    //}

    bool hasSwitchedSource = false;

    if (mapPlot->failedToFetchTiles() && !sourceIsFs) {
        sourceIsFs = true;
        hasSwitchedSource = true;
        GCS_LOG_DEBUG("Lost connection to tile provider.");
    }

    if (sourceIsFs) {
        if (urlConnectionTest->canFetch()) {
            sourceIsFs = false;
            hasSwitchedSource = true;
            GCS_LOG_DEBUG("Regained connection to tile provider.");
        }
        urlConnectionTest->testFetch();
    }

    if (mapView != prevMapView) {
        if (sourceIsFs) {
            std::string mapViewFolder;
            if (mapView == MAP_VIEW) {
                mapViewFolder = "map";
            } else if (mapView == SATELLITE_VIEW) {
                mapViewFolder = "satellite";
            }
            std::string tileDir = (std::filesystem::current_path() / "tiles" / mapViewFolder).string();
            mapPlot->setTileLoader(std::make_shared<TileLoaderFsMap>(tileDir));
        } else {
            if (mapView == MAP_VIEW) {
                mapPlot->setTileLoader(std::make_shared<TileLoaderOsmMap>());
            } else if (mapView == SATELLITE_VIEW) {
                mapPlot->setTileLoader(std::make_shared<TileLoaderArcMap>());
            }
        }
        prevMapView = mapView;
    }

    mapPlot->paint();
}

void MapWindow::loadState(const mINI::INIStructure& ini) {
    mapPlot = std::make_shared<RichMapPlot>();
    storage = std::make_shared<MarkStorage>();

    std::string tileDir = (std::filesystem::current_path() / "tiles" / "map").string();
    tileGrabber = std::make_shared<TileGrabber>(
        std::make_shared<TileSourceUrlOsm>(URL_REQUEST_LIMIT, MAP_PRELOAD),
        std::make_shared<TileSaverSubDir>(tileDir)
    );

    urlConnectionTest = std::make_shared<TileSourceUrlOsm>(URL_REQUEST_LIMIT, MAP_PRELOAD);

    storage->addMark({46.14665264871996, -70.66861153239353}, "Tapis Venture");
    mapPlot->addItem(std::reinterpret_pointer_cast<IRichItem>(storage->markItems().back().ptr));

    mapPlot->loadState(ini);
    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW)) {
            mapView = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW));
        }
    }
}

void MapWindow::saveState(mINI::INIStructure& ini) {
    mapPlot->saveState(ini);
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_MAP_WINDOW_MAP_VIEW, std::to_string(mapView));
}

void MapWindow::checkTileSourceConnection() {
    
}
