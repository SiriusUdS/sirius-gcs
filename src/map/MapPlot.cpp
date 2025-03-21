#include "MapPlot.h"

#include "Coords.h"
#include "TileLoaderImpl.h"

#include <algorithm>
#include <implot.h>

/**
 * @struct MapPlot::Impl
 * @brief Contains specific data to the implementation of the map plot ImGui component
 */
struct MapPlot::Impl {
    constexpr static const ImPlotFlags plotFlags{ImPlotFlags_Equal | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText
                                                 | ImPlotFlags_NoMenus}; ///< ImGui plot flags

    constexpr static const ImPlotAxisFlags xFlags{ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks
                                                  | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_NoMenus
                                                  | ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoHighlight}; ///< ImGui plot x axis flags

    constexpr static const ImPlotAxisFlags yFlags{xFlags | ImPlotAxisFlags_Invert}; ///< ImGui plot y axis flags

    constexpr static const ImVec2 uv0{0, 1},        ///< Texture coordinates (top-left corner)
      uv1{1, 0};                                    ///< Texture coordinates (bottom-right corner)
    constexpr static const ImVec4 tint{1, 1, 1, 1}; ///< Texture tint

    ImPlotPoint mousePos{}; ///< The mouse cursor position.
    ImPlotRect plotLims{};  ///< The current plot axis range.
    ImVec2 plotSize{};      ///< The current plot size.
    ImVec2 plotPos{};       ///< The current plot position.
};

/**
 * @brief Constructs a map plot with a default tile loader.
 */
MapPlot::MapPlot() : _impl{std::make_unique<Impl>()}, _loader{std::make_shared<TileLoaderOsmMap>()} {
}

/**
 * @brief Constructs a map plot with the tile loader passed as a parameter.
 */
MapPlot::MapPlot(std::shared_ptr<ITileLoader>& loader)
    : _impl{std::make_unique<Impl>()}, _loader{loader}, _minLat{(float) MIN_LAT}, _minLon{(float) MIN_LON}, _maxLat{(float) MAX_LAT},
      _maxLon{(float) MAX_LON} {
    resetBounds();
}

MapPlot::~MapPlot() = default;

/**
 * @brief Displays the map.
 */
void MapPlot::paint() {
    if (ImPlot::BeginPlot("##ImOsmMapPlot", {-1, -1}, _impl->plotFlags)) {
        ImPlot::SetupAxis(ImAxis_X1, nullptr, _impl->xFlags);
        ImPlot::SetupAxis(ImAxis_Y1, nullptr, _impl->yFlags);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

        if (_setBounds != SetBounds::None) {
            if (_setBounds == SetBounds::Geo) {
                _minX = (float) lon2x(_minLon, 0);
                _maxX = (float) lon2x(_maxLon, 0);
                _minY = (float) lat2y(_minLat, 0);
                _maxY = (float) lat2y(_maxLat, 0);
            } else if (_setBounds == SetBounds::Local) {
                // do nothing
            }
            ImPlot::SetupAxisLimits(ImAxis_X1, _minX, _maxX, ImPlotCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, _minY, _maxY, ImPlotCond_Always);
            _setBounds = SetBounds::None;
        }

        ImPlot::SetupFinish();

        _impl->mousePos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);
        _impl->plotLims = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
        _impl->plotSize = ImPlot::GetPlotSize();
        _impl->plotPos = ImPlot::GetPlotPos();

        _mouseLon = (float) x2lon(_impl->mousePos.x, 0);
        _mouseLat = (float) y2lat(_impl->mousePos.y, 0);

        _pixelsX = _impl->plotSize.x;
        _pixelsY = _impl->plotSize.y;

        _minX = (float) _impl->plotLims.X.Min;
        _maxX = (float) _impl->plotLims.X.Max;
        _minY = (float) _impl->plotLims.Y.Min;
        _maxY = (float) _impl->plotLims.Y.Max;
        _rangeX = fabs(_maxX - _minX);
        _rangeY = fabs(_maxY - _minY);

        _resX = _pixelsX / _rangeX;
        _resY = _pixelsY / _rangeY;
        _zoom = std::clamp(int(floor(log2(_resX / _tilePixels))), MIN_ZOOM, MAX_ZOOM);
        _tilesNum = POW2[_zoom];
        _tileSize = 1.0f / float(_tilesNum);

        const auto minMaxLat{std::minmax({y2lat(_minY * _tilesNum, _zoom), y2lat(_maxY * _tilesNum, _zoom)})};
        const auto minMaxLon{std::minmax({x2lon(_minX * _tilesNum, _zoom), x2lon(_maxX * _tilesNum, _zoom)})};

        _minLat = (float) minMaxLat.first;
        _maxLat = (float) minMaxLat.second;
        _minLon = (float) minMaxLon.first;
        _maxLon = (float) minMaxLon.second;

        _minTX = std::clamp(int(_minX * _tilesNum), 0, _tilesNum - 1);
        _maxTX = std::clamp(int(_maxX * _tilesNum), 0, _tilesNum - 1);
        _minTY = std::clamp(int(_minY * _tilesNum), 0, _tilesNum - 1);
        _maxTY = std::clamp(int(_maxY * _tilesNum), 0, _tilesNum - 1);

        _loader->beginLoad(_zoom, _minTX, _maxTX, _minTY, _maxTY);

        ImVec2 bmin{float(_minTX), float(_minTY)};
        ImVec2 bmax{float(_maxTX), float(_maxTY)};

        for (auto x{_minTX}; x != _maxTX + 1; ++x) {
            bmin.x = float(x) * _tileSize;
            bmax.x = float(x + 1) * _tileSize;
            for (auto y{_minTY}; y != _maxTY + 1; ++y) {
                bmin.y = float(y) * _tileSize;
                bmax.y = float(y + 1) * _tileSize;
                ImPlot::PlotImage("##", _loader->tileAt(_zoom, x, y), bmin, bmax, MapPlot::Impl::uv0, MapPlot::Impl::uv1, MapPlot::Impl::tint);
            }
        }

        _loader->endLoad();

        paintOverMap();

        ImPlot::EndPlot();
    }
}

/**
 * @brief Reset the latitude/longitude map bounds to display the entire world map.
 */
void MapPlot::resetBounds() {
    _minLat = (float) MIN_LAT;
    _maxLat = (float) MAX_LAT;
    _minLon = (float) MIN_LON;
    _maxLon = (float) MAX_LON;
    _setBounds = SetBounds::Geo;
}

/**
 * @brief Set specific latitude/longitude map bounds.
 */
void MapPlot::setBoundsGeo(float minLat, float maxLat, float minLon, float maxLon) {
    _minLat = minLat;
    _maxLat = maxLat;
    _minLon = minLon;
    _maxLon = maxLon;
    _setBounds = SetBounds::Geo;
}

/**
 * @brief Get the latitude/longitude map bounds.
 */
void MapPlot::getBoundsGeo(float& minLat, float& maxLat, float& minLon, float& maxLon) const {
    minLat = _minLat;
    maxLat = _maxLat;
    minLon = _minLon;
    maxLon = _maxLon;
}

/**
 * @brief Get the tile map bounds.
 */
void MapPlot::getBoundsTile(int& minTX, int& maxTX, int& minTY, int& maxTY) const {
    minTX = _minTX;
    maxTX = _maxTX;
    minTY = _minTY;
    maxTY = _maxTY;
}

/**
 * @brief Set the local map bounds.
 */
void MapPlot::setBoundsLocal(float minX, float maxX, float minY, float maxY) {
    _minX = minX;
    _maxX = maxX;
    _minY = minY;
    _maxY = maxY;
    _setBounds = SetBounds::Local;
}

/**
 * @brief Get the local map bounds.
 */
void MapPlot::getBoundsLocal(float& minX, float& maxX, float& minY, float& maxY) const {
    minX = _minX;
    maxX = _maxX;
    minY = _minY;
    maxY = _maxY;
}

/**
 * @brief Return true if the mouse is on the plot.
 */
bool MapPlot::mouseOnPlot() const {
    return _impl->mousePos.x > _impl->plotLims.X.Min && _impl->mousePos.x < _impl->plotLims.X.Max && _impl->mousePos.y > _impl->plotLims.Y.Min
           && _impl->mousePos.y < _impl->plotLims.Y.Max;
}

/**
 * @brief Return true if the map plot has recently failed to fetch multiple tiles.
 */
bool MapPlot::failedToFetchTiles() const {
    return _loader->failedLoad();
}
