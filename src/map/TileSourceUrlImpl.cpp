#include "TileSourceUrlImpl.h"

#include <sstream>

/**
 * @brief Constructs a custom url tile source
 * @param request_limit Maximum number of concurrent requests that can be made to fetch tiles from the custom url
 * @param preload Preload tile image
 * @param url_tpl Template url which contains the x, y, z parameters to be replaced. The url template should contain $Z$, $X$ and $Y$ for the z, x and
 * y params respectively.
 */
TileSourceUrlCustom::TileSourceUrlCustom(int request_limit, bool preload, const std::string& url_tpl)
    : TileSourceUrl{request_limit, preload}, _url_tpl{url_tpl} {
}

std::string TileSourceUrlCustom::makeUrl(int z, int x, int y) {
    std::string url{_url_tpl};
    const auto pos_z{url.find("$Z$")};
    if (pos_z < url.size()) {
        url.replace(pos_z, 3, std::to_string(z));
        const auto pos_x{url.find("$X$")};
        if (pos_x < url.size()) {
            url.replace(pos_x, 3, std::to_string(x));
            const auto pos_y{url.find("$Y$")};
            if (pos_y < url.size()) {
                url.replace(pos_y, 3, std::to_string(y));
            }
        }
    }
    return url;
}

// -----------------------------------------------------------------------------

/**
 * @brief Constructs an OSM url tile source
 * @param request_limit Maximum number of concurrent requests that can be made to fetch osm tiles
 * @param preload Preload tile images
 */
TileSourceUrlOsm::TileSourceUrlOsm(int request_limit, bool preload) : TileSourceUrl{request_limit, preload} {
}

std::string TileSourceUrlOsm::makeUrl(int z, int x, int y) {
    return TileSourceUrlOsm::makeSourceUrl(z, x, y);
}

/**
 * @brief Construct tile source url to fetch a specific OSM tile
 * @param z Zoom level of the tile
 * @param x X position of the tile
 * @param y Y position of the tile
 * @returns Url to fetch the tile
 */
std::string TileSourceUrlOsm::makeSourceUrl(int z, int x, int y) {
    std::ostringstream urlmaker;
    urlmaker << "https://a.tile.openstreetmap.org";
    urlmaker << '/' << z << '/' << x << '/' << y << ".png";
    return urlmaker.str();
}

// -----------------------------------------------------------------------------

/**
 * @brief Constructs an ARC imagery url tile source
 * @param request_limit Maximum number of concurrent requests that can be made to fetch arc tiles
 * @param preload Preload tile images
 */
TileSourceUrlArcImagery::TileSourceUrlArcImagery(int request_limit, bool preload) : TileSourceUrl{request_limit, preload} {
}

std::string TileSourceUrlArcImagery::makeUrl(int z, int x, int y) {
    return TileSourceUrlArcImagery::makeSourceUrl(z, x, y);
}

/**
 * @brief Construct tile source url to fetch a specific Arc tile
 * @param z Zoom level of the tile
 * @param x X position of the tile
 * @param y Y position of the tile
 * @returns Url to fetch the tile
 */
std::string TileSourceUrlArcImagery::makeSourceUrl(int z, int x, int y) {
    std::ostringstream urlmaker;
    urlmaker << "https://server.arcgisonline.com/ArcGIS/rest/"
                "services/World_Imagery/MapServer/tile";
    urlmaker << '/' << z << '/' << y << '/' << x;
    return urlmaker.str();
}
