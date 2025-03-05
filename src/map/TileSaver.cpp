#include "TileSaver.h"

#include "ITile.h"

#include <fstream>
#include <string>

TileSaver::TileSaver() = default;
TileSaver::~TileSaver() = default;

/**
 * @brief Saves multiple tiles
 * @param tiles Tiles to be saved
 * @return bool All tiles have been successfully saved
 */
bool TileSaver::saveMulti(const std::vector<std::shared_ptr<ITile>>& tiles) const {
    for (const auto& tile : tiles) {
        if (!save(tile)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Saves a single tile
 * @param tile Tile to be saved
 * @return bool Tile was successfully saved
 */
bool TileSaver::save(std::shared_ptr<ITile> tile) const {
    auto path{dirPath(tile)};
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
    std::ofstream file_maker(path.append(fileName(tile)), std::fstream::out | std::fstream::binary);
    if (file_maker) {
        file_maker.write(tile->rawBlob(), tile->rawBlobSize());
        return true;
    }
    return false;
}

/**
 * @brief Gets file name of specific tile
 * @param tile Tile to generate file name from
 * @return std::string
 */
std::string TileSaver::fileName(std::shared_ptr<ITile> tile) const {
    return TileSourceFs::FileName(tile->z(), tile->x(), tile->y());
}
// -----------------------------------------------------------------------------

TileSaverDir::TileSaverDir() = default;

/**
 * @brief Constructs tile saver with a specific directory
 * @param basePath Directory path to save tiles at
 */
TileSaverDir::TileSaverDir(const std::filesystem::path& basePath) : _basePath{basePath} {
}

std::filesystem::path TileSaverDir::dirPath(std::shared_ptr<ITile> tile) const {
    return TileSourceFsDir::DirPath(_basePath, tile->z(), tile->x(), tile->y());
}

TileSaverDir::~TileSaverDir() = default;

// -----------------------------------------------------------------------------

TileSaverSubDir::TileSaverSubDir() = default;

/**
 * @brief Constructs tile saver with a specific directory
 * @param basePath Directory path to save tiles at
 */
TileSaverSubDir::TileSaverSubDir(const std::filesystem::path& basePath) : _basePath{basePath} {
}

TileSaverSubDir::~TileSaverSubDir() = default;

std::filesystem::path TileSaverSubDir::dirPath(std::shared_ptr<ITile> tile) const {
    return TileSourceFsSubDir::DirPath(_basePath, tile->z(), tile->x(), tile->y());
}
