#include "RichMarkStorage.h"

#include "RichMarkItem.h"

MarkStorage::MarkStorage() = default;

MarkStorage::~MarkStorage() = default;

/**
 * @brief Finds a marker by name in the storage
 * @param name Name of the marker to find
 * @return If mark found, the shared_ptr of the mark item, otherwise nullptr
 */
std::shared_ptr<MarkItem> MarkStorage::findMark(const std::string& name) const {
    const auto it{std::find_if(_markItems.begin(), _markItems.end(), [name](const ItemNode& node) { return node.ptr->text() == name; })};
    if (it != _markItems.end()) {
        return it->ptr;
    }
    return nullptr;
}

/**
 * @brief Finds a mark by name in the storage and returns its longitude and latitude
 * @param name The name of the mark item to get the GeoCoords from
 * @param ok Set to true if the mark item was found, otherwise set to false
 * @returns The GeoCoords of the mark item if one was found, else empty GeoCoords are returned (all values set to 0)
 */
GeoCoords MarkStorage::findMark(const std::string& name, bool& ok) const {
    const auto ptr{findMark(name)};
    if (ptr) {
        ok = true;
        return ptr->geoCoords();
    }
    ok = false;
    return {};
}

/**
 * @brief Load the state of the mark item storage from an ini struct
 * @param ini The ini struct to load the state from
 */
void MarkStorage::loadState(const mINI::INIStructure& ini) {
    for (auto it{ini.begin()}; it != ini.end(); ++it) {
        if (it->first.starts_with("mark_")) {
            auto ptr{std::make_shared<MarkItem>()};
            if (it->second.has("text")) {
                ptr->setText(it->second.get("text"));
            }
            if (it->second.has("lat") && it->second.has("lon")) {
                ptr->setCoords({std::stof(it->second.get("lat")), std::stof(it->second.get("lon"))});
            }
            if (it->second.has("radius")) {
                ptr->setRadius(std::stof(it->second.get("radius")));
            }
            if (it->second.has("text_enabled")) {
                ptr->style().textEnabled = std::stoi(it->second.get("text_enabled"));
            }
            if (it->second.has("marker_enabled")) {
                ptr->style().markerEnabled = std::stoi(it->second.get("marker_enabled"));
            }
            if (it->second.has("radius_enabled")) {
                ptr->style().radiusEnabled = std::stoi(it->second.get("radius_enabled"));
            }
            if (it->second.has("radius_weight")) {
                ptr->style().radiusWeight = std::stof(it->second.get("radius_weight"));
            }
            if (it->second.has("marker_type")) {
                ptr->style().markerType = std::stoi(it->second.get("marker_type"));
            }
            if (it->second.has("marker_size")) {
                ptr->style().markerSize = std::stof(it->second.get("marker_size"));
            }
            if (it->second.has("marker_weight")) {
                ptr->style().markerWeight = std::stof(it->second.get("marker_weight"));
            }
            if (it->second.has("marker_fill_x") && it->second.has("marker_fill_y") && it->second.has("marker_fill_z")
                && it->second.has("marker_fill_w")) {
                ptr->style().markerFill = {std::stof(it->second.get("marker_fill_x")), std::stof(it->second.get("marker_fill_y")),
                                           std::stof(it->second.get("marker_fill_z")), std::stof(it->second.get("marker_fill_w"))};
            }

            _markItems.push_back({ptr});
        }
    }
}

/**
 * @brief Save the state of the mark item storage to an ini struct
 * @param ini The ini struct to save the mark item storage state to
 */
void MarkStorage::saveState(mINI::INIStructure& ini) const {
    static const auto key_base{"mark_"};
    unsigned index{};
    for (const auto& item : _markItems) {
        const std::string key{key_base + std::to_string(index++)};
        ini[key]["text"] = item.ptr->text();
        ini[key]["lat"] = std::to_string(item.ptr->geoCoords().lat);
        ini[key]["lon"] = std::to_string(item.ptr->geoCoords().lon);
        ini[key]["radius"] = std::to_string(item.ptr->radius());
        ini[key]["text_enabled"] = std::to_string(item.ptr->style().textEnabled);
        ini[key]["marker_enabled"] = std::to_string(item.ptr->style().markerEnabled);
        ini[key]["radius_enabled"] = std::to_string(item.ptr->style().radiusEnabled);
        ini[key]["radius_weight"] = std::to_string(item.ptr->style().radiusWeight);
        ini[key]["marker_type"] = std::to_string(item.ptr->style().markerType);
        ini[key]["marker_size"] = std::to_string(item.ptr->style().markerSize);
        ini[key]["marker_weight"] = std::to_string(item.ptr->style().markerWeight);
        ini[key]["marker_fill_x"] = std::to_string(item.ptr->style().markerFill.x);
        ini[key]["marker_fill_y"] = std::to_string(item.ptr->style().markerFill.y);
        ini[key]["marker_fill_z"] = std::to_string(item.ptr->style().markerFill.z);
        ini[key]["marker_fill_w"] = std::to_string(item.ptr->style().markerFill.w);
    }
}

/**
 * @brief Add a new mark item to the mark item storage
 * @param coords The longitude and latitude of the new mark item to add
 * @param name The name of the new mark item to add
 */
void MarkStorage::addMark(const GeoCoords& coords, const std::string& name) {
    _markItems.push_back({std::make_shared<MarkItem>(coords, name)});
}

/**
 * @brief Remove mark items if their rmFlag is set to true
 */
void MarkStorage::rmMarks() {
    _markItems.erase(std::remove_if(_markItems.begin(), _markItems.end(), [](auto& item) { return item.rmFlag; }), _markItems.end());
}
