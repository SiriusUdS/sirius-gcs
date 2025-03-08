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
int downloadMinZ{0};
int downloadMaxZ{18};
size_t downloadTileCount{0};
size_t downloadTileTotal{};
float downloadProgress{0};
std::chrono::seconds autoSourceSwitchDelay{3};
std::chrono::steady_clock::time_point lastAutoSourceSwitchTime{std::chrono::steady_clock::now()};
std::shared_ptr<RichMapPlot> mapPlot;
std::shared_ptr<MarkStorage> storage;
std::shared_ptr<TileGrabber> mapTileGrabber;
std::shared_ptr<TileGrabber> satelliteTileGrabber;
std::shared_ptr<TileSourceUrl> urlConnectionTest;
} // namespace MapWindow

void MapWindow::render() {
    if (ImGui::CollapsingHeader("Download Tiles")) {
        ImGui::Indent(20.0f);

        size_t tilesSelectedTotal =
          2 * countTiles(mapPlot->minLat(), mapPlot->maxLat(), mapPlot->minLon(), mapPlot->maxLon(), downloadMinZ, downloadMaxZ);
        bool maxTilesDownloadExceeded = tilesSelectedTotal > Constants::GCS_MAP_MAX_TILES_DOWNLOAD;

        // Latitude and longitude
        ImGui::Text("Min Lat %.6f, Min Lon %.6f", mapPlot->minLat(), mapPlot->minLon());
        ImGui::Text("Max Lat %.6f, Max Lon %.6f", mapPlot->maxLat(), mapPlot->maxLon());

        // Input to set which zoom levels we want to download the tiles from + tile counter
        ImGui::SetNextItemWidth(200);
        ImGui::InputInt("Min Z", &downloadMinZ);
        downloadMinZ = std::clamp(downloadMinZ, 0, MAX_ZOOM);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputInt("Max Z", &downloadMaxZ);
        downloadMaxZ = std::clamp(downloadMaxZ, 0, MAX_ZOOM);

        // Tile count and zoom level
        if (maxTilesDownloadExceeded) {
            ImGui::Text("Tiles Count:");
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text(">%lu (Too many tiles, can't download)", Constants::GCS_MAP_MAX_TILES_DOWNLOAD);
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("Tiles Count: %lu", tilesSelectedTotal);
        }
        ImGui::SameLine();
        ImGui::Text("Current Zoom Level: %d", mapPlot->zoom());

        // Download button
        ImGui::BeginDisabled(sourceIsFs || maxTilesDownloadExceeded || !mapTileGrabber->done() || !satelliteTileGrabber->done());
        if (ImGui::Button("Download")) {
            downloadTileTotal = tilesSelectedTotal;
            mapTileGrabber->grab(mapPlot->minLat(), mapPlot->maxLat(), mapPlot->minLon(), mapPlot->maxLon(), downloadMinZ, downloadMaxZ);
            satelliteTileGrabber->grab(mapPlot->minLat(), mapPlot->maxLat(), mapPlot->minLon(), mapPlot->maxLon(), downloadMinZ, downloadMaxZ);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            if (sourceIsFs) {
                ImGui::SetTooltip(
                  "Map tiles cannot be downloaded because the tile provider cannot be accessed (likely because of no Internet access).");
            } else if (maxTilesDownloadExceeded) {
                ImGui::SetTooltip("Downloads exceeding %lu tiles are not permitted.", Constants::GCS_MAP_MAX_TILES_DOWNLOAD);
            } else {
                ImGui::SetTooltip("Download map tiles that are currently on screen so they can be accessed without having to fetch them online.");
            }
        }
        ImGui::EndDisabled();
        ImGui::SameLine();

        // Stop download button
        ImGui::BeginDisabled((mapTileGrabber->done() || mapTileGrabber->isStopping())
                             && (satelliteTileGrabber->done() || satelliteTileGrabber->isStopping()));
        if (ImGui::Button("Stop")) {
            mapTileGrabber->stop();
            satelliteTileGrabber->stop();
        }
        ImGui::EndDisabled();

        // Download progress bar
        downloadTileCount = mapTileGrabber->tileCounter() + satelliteTileGrabber->tileCounter();
        downloadProgress = float(downloadTileCount) / float(downloadTileTotal);
        downloadProgress = std::isnan(downloadProgress) ? 0.f : downloadProgress;
        ImGui::SameLine();
        ImGui::Text("%d/%d", downloadTileCount, downloadTileTotal);
        ImGui::SameLine();
        ImGui::ProgressBar(downloadProgress);

        ImGui::Unindent(20.0f);
    }

    ImGui::TextUnformatted("View type: ");
    ImGui::SameLine();
    ImGui::RadioButton("Map", &mapView, MAP_VIEW);
    ImGui::SameLine();
    ImGui::RadioButton("Satellite", &mapView, SATELLITE_VIEW);

    bool hasSwitchedSource = mapView != prevMapView;

    auto now = std::chrono::steady_clock::now();
    bool autoSourceSwitchDelayElapsed = now - lastAutoSourceSwitchTime > autoSourceSwitchDelay;

    // Check if we can fetch tiles from online provider
    // If we can't, fetch the downloaded tiles locally
    if (mapPlot->failedToFetchTiles() && autoSourceSwitchDelayElapsed && !sourceIsFs) {
        sourceIsFs = true;
        lastAutoSourceSwitchTime = now;
        hasSwitchedSource = true;
        GCS_LOG_DEBUG("MapWindow: Lost connection to tile provider.");
    } else if (sourceIsFs) {
        if (urlConnectionTest->canFetch() && autoSourceSwitchDelayElapsed) {
            sourceIsFs = false;
            lastAutoSourceSwitchTime = now;
            hasSwitchedSource = true;
            GCS_LOG_DEBUG("MapWindow: Regained connection to tile provider.");
        }
        urlConnectionTest->startConnectivityTest();
    }

    if (hasSwitchedSource) {
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
    urlConnectionTest = std::make_shared<TileSourceUrlOsm>(URL_REQUEST_LIMIT, MAP_PRELOAD);

    const int TILE_REQUEST_LIMIT = 25;
    std::string mapTileDir = (std::filesystem::current_path() / "tiles" / "map").string();
    std::string satelliteTileDir = (std::filesystem::current_path() / "tiles" / "satellite").string();
    mapTileGrabber = std::make_shared<TileGrabber>(std::make_shared<TileSourceUrlOsm>(TILE_REQUEST_LIMIT, MAP_PRELOAD),
                                                   std::make_shared<TileSaverSubDir>(mapTileDir));
    satelliteTileGrabber = std::make_shared<TileGrabber>(std::make_shared<TileSourceUrlArc>(TILE_REQUEST_LIMIT, MAP_PRELOAD),
                                                         std::make_shared<TileSaverSubDir>(satelliteTileDir));

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
