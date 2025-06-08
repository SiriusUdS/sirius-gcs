#include "TileLoader.h"

#include "ITile.h"
#include "ITileSource.h"

#include <algorithm>

/**
 * @brief Constructs a tile loader from a tile source
 * @param source Tile source to load the tiles from
 * @param dummiesAreFinal If a tile can't be fetched it is replaced by a dummy tile. Setting this to true means that no additionnal attempts will be
 * made to fetch the tile if it is a dummy.
 */
TileLoader::TileLoader(std::shared_ptr<ITileSource> source, bool dummiesAreFinal) : _source{source}, _dummiesAreFinal{dummiesAreFinal} {
}

void TileLoader::beginLoad(int z, int xmin, int xmax, int ymin, int ymax) {
    const auto cond{[this, z, xmin, xmax, ymin, ymax](const std::shared_ptr<ITile>& tile) {
        return !tile->inBounds(z, xmin, xmax, ymin, ymax) || (tile->isDummy() && !_dummiesAreFinal);
    }};
    _tiles.erase(std::remove_if(_tiles.begin(), _tiles.end(), cond), _tiles.end());
    _source->takeReady(_tiles);
}

ImTextureID TileLoader::tileAt(int z, int x, int y) {
    const auto cond{[z, x, y](const std::shared_ptr<ITile>& tile) { return tile->isTileZXY(z, x, y); }};
    const auto it{std::find_if(_tiles.begin(), _tiles.end(), cond)};

    if (it != _tiles.end()) {
        if (!(*it)->isDummy() || _dummiesAreFinal) {
            return (*it)->texture();
        }
    }

    if (!_source->hasRequest(z, x, y)) {
        _source->request(z, x, y);
    }

    return 0;
}

bool TileLoader::failedLoad() {
    return _source->hasFailedManyRequests();
}
