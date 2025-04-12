#include "RichMapPlot.h"

#include "IRichItem.h"

RichMapPlot::RichMapPlot() {
}

RichMapPlot::~RichMapPlot() = default;

void RichMapPlot::paintOverMap() {
    MapPlot::paintOverMap();

    _items.erase(std::remove_if(_items.begin(), _items.end(), [](auto item) { return item.expired(); }), _items.end());

    std::for_each(_items.begin(), _items.end(), [this](auto ptr) {
        auto item{ptr.lock()};
        if (item->enabled() && item->inBounds(minLat(), maxLat(), minLon(), maxLon()))
            item->paint();
    });
}

/**
 * @brief Loads the state of the rich map plot from an ini struct
 * @param ini The ini struct to load the state from
 */
void RichMapPlot::loadState(const mINI::INIStructure& ini) {
    if (ini.has("map_plot")) {
        if (ini.get("map_plot").has("min_lat") && ini.get("map_plot").has("max_lat") && ini.get("map_plot").has("min_lon")
            && ini.get("map_plot").has("max_lon")) {
            setBoundsGeo(std::stof(ini.get("map_plot").get("min_lat")), std::stof(ini.get("map_plot").get("max_lat")),
                         std::stof(ini.get("map_plot").get("min_lon")), std::stof(ini.get("map_plot").get("max_lon")));
        }
    }
}

/**
 * @brief Saves the state of the rich map plot to an ini struct
 * @param ini The ini struct to save the state to
 */
void RichMapPlot::saveState(mINI::INIStructure& ini) const {
    float minLat{}, maxLat{}, minLon{}, maxLon{};
    getBoundsGeo(minLat, maxLat, minLon, maxLon);
    ini["map_plot"].set("min_lat", std::to_string(minLat));
    ini["map_plot"].set("max_lat", std::to_string(maxLat));
    ini["map_plot"].set("min_lon", std::to_string(minLon));
    ini["map_plot"].set("max_lon", std::to_string(maxLon));
}

/**
 * @brief Add a rich item to the rich map plot
 * @param item The item to add
 */
void RichMapPlot::addItem(std::weak_ptr<IRichItem> item) {
    _items.push_back(item);
}
