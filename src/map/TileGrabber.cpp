#include "TileGrabber.h"

#include "Coords.h"
#include "ITile.h"
#include "ITileSaver.h"
#include "ITileSource.h"

/**
 * @brief Constructs a tile grabber from a source and a saver
 */
TileGrabber::TileGrabber(std::shared_ptr<ITileSource> source, std::shared_ptr<ITileSaver> saver) : _source{source}, _saver{saver} {
}

TileGrabber::~TileGrabber() {
}

/**
 * @brief Grabs all the tiles in the specified zone at the specified zoom levels
 * @param minLat Minimal latitude
 * @param maxLat Maximal latitude
 * @param minLon Minimal longitude
 * @param maxLon Maximal longitude
 * @param minZ Minimal zoom level
 * @param maxZ Maximal zoom level
 */
void TileGrabber::grab(const double minLat, const double maxLat, const double minLon, const double maxLon, const int minZ, const int maxZ) {
    _stop = false;
    _future = std::async(std::launch::async, &TileGrabber::onLaunchGrab, this, minLat, maxLat, minLon, maxLon, minZ, maxZ);
}

/**
 * @brief Stops the tile grabber from currently grabbing anymore tiles
 */
void TileGrabber::stop() {
    _stop = true;
}

/**
 * @brief Returns whether the tile grabber is stopping
 * @returns True if stopping, else false
 */
bool TileGrabber::isStopping() {
    return _stop;
}

/**
 * @brief Returns whether the tile grabber is done fetching tiles and saving them
 * @returns True if done, else false
 */
bool TileGrabber::done() {
    return _isDone;
}

/**
 * @brief The asynchronously executed task to fetch the tiles in the specified zone at the specified zoom levels
 * @param minLat Minimal latitude
 * @param maxLat Maximal latitude
 * @param minLon Minimal longitude
 * @param maxLon Maximal longitude
 * @param minZ Minimal zoom level
 * @param maxZ Maximal zoom level
 */
TileGrabber::FutureData TileGrabber::onLaunchGrab(const double minLat, const double maxLat, const double minLon, const double maxLon, const int minZ,
                                                  const int maxZ) {
    FutureData data;

    if (!_isDone) {
        return data;
    }

    _isDone = false;
    _tileCounter = 0;
    std::vector<std::shared_ptr<ITile>> tiles;
    tiles.reserve(_source->requestLimit());

    for (auto z{minZ}; z != maxZ + 1; ++z) {
        const auto tx{minmax_tx(minLon, maxLon, z)};
        const auto ty{minmax_ty(minLat, maxLat, z)};
        for (auto x{tx.first}; x != tx.second + 1; ++x) {
            for (auto y{ty.first}; y != ty.second + 1; ++y) {
                if (_stop) {
                    _isDone = true;
                    return data;
                }
                if (!_source->canRequest()) {
                    tiles.clear();
                    _source->takeAll(tiles);
                    std::for_each(std::begin(tiles), std::end(tiles), [this](const auto& tile) {
                        if (tile->isDummy()) {
                            _source->request(tile->z(), tile->x(), tile->y());
                        }
                    });
                    std::erase_if(tiles, [](const auto& tile) { return tile->isDummy(); });
                    _source->takeAll(tiles);
                    _saver->saveMulti(tiles);
                    _tileCounter += (int) tiles.size();
                }
                _source->request(z, x, y);
            }
        }
    }

    tiles.clear();
    _source->takeAll(tiles);
    _saver->saveMulti(tiles);
    _tileCounter += (int) tiles.size();

    _isDone = true;
    return data;
}
