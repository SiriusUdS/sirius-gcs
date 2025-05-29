#include "TileSourceFs.h"

#include <fstream>

/**
 * @brief Constructs a file system tile source
 * @param requestLimit Max number of concurrent requests to fetch tiles from filesystem
 * @param preload Preloads tile image
 * @param basePath Path of the folder in which the tiles will be fetched
 */
TileSourceFs::TileSourceFs(int requestLimit, bool preload, const std::filesystem::path& basePath)
    : TileSourceAsync{requestLimit, preload}, _basePath{basePath} {
}

/**
 * @brief Generates the file name for a specific tile
 * @param z Zoom level of the tile
 * @param x X position of the tile
 * @param y Y position of the tile
 * @returns The file name of the tile
 */
std::string TileSourceFs::FileName(int z, int x, int y) {
    std::ostringstream fname_maker;
    fname_maker << z << '-' << x << '-' << y;
    return fname_maker.str();
}

/**
 * @brief Returns the default base path of the folder in which the tile files will be fetched
 * @returns Filesystem path
 */
std::filesystem::path TileSourceFs::BasePathDefault() {
    return std::filesystem::current_path().append("tiles");
}

bool TileSourceFs::receiveTile(int z, int x, int y, TileData& tileData) {
    auto path{dirPath(z, x, y)};
    std::ifstream reader(path.append(FileName(z, x, y)), std::fstream::in | std::fstream::binary | std::ios::ate);
    auto pos{reader.tellg()};
    if (reader && pos > 0) {
        tileData.blob.resize(pos);
        reader.seekg(0, std::ios::beg);
        reader.read(reinterpret_cast<char*>(&tileData.blob[0]), pos);
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------

/**
 * @brief Constructs a file system directory tile source
 * @param requestLimit Max number of concurrent requests to fetch tiles from filesystem
 * @param preload Preloads tile image
 * @param basePath Path of the folder in which the tiles will be fetched
 */
TileSourceFsDir::TileSourceFsDir(int requestLimit, bool preload, const std::filesystem::path& basePath)
    : TileSourceFs{requestLimit, preload, basePath} {
}

std::filesystem::path TileSourceFsDir::dirPath(int z, int x, int y) const {
    return DirPath(basePath(), z, x, y);
}
// -----------------------------------------------------------------------------

/**
 * @brief Constructs a file system subdirectory tile source
 * @param requestLimit Max number of concurrent requests to fetch tiles from filesystem
 * @param preload Preloads tile image
 * @param basePath Path of the folder in which the tiles will be fetched
 */
TileSourceFsSubDir::TileSourceFsSubDir(int requestLimit, bool preload, const std::filesystem::path& basePath)
    : TileSourceFs{requestLimit, preload, basePath} {
}

/**
 * @brief Returns the current path of the folder in which the tile files will be fetched (with zoom level subfolder)
 * @returns Filesystem path
 */
std::filesystem::path TileSourceFsSubDir::DirPath(std::filesystem::path basePath, int z, int x, int y) {
    return basePath.append(std::to_string(z));
}

std::filesystem::path TileSourceFsSubDir::dirPath(int z, int x, int y) const {
    return DirPath(basePath(), z, x, y);
}
