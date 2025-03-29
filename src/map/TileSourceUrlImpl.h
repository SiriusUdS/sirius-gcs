#ifndef TILESOURCEURLIMPL_H
#define TILESOURCEURLIMPL_H

#include "TileSourceUrl.h"

// https://www.arcgis.com/apps/mapviewer/index.html
// See Basemap

class TileSourceUrlCustom : public TileSourceUrl {
public:
    TileSourceUrlCustom(int request_limit, bool preload, const std::string& url_tpl);

protected:
    virtual std::string makeUrl(int z, int x, int y) override;

private:
    const std::string _url_tpl;
};

// -----------------------------------------------------------------------------

class TileSourceUrlOsm : public TileSourceUrl {
public:
    TileSourceUrlOsm(int request_limit, bool preload);

protected:
    virtual std::string makeUrl(int z, int x, int y) override;

public:
    inline static const char URL_TPL[]{"https://a.tile.openstreetmap.org/$Z$/$X$/$Y$.png"};

    static std::string makeSourceUrl(int z, int x, int y);
};

// -----------------------------------------------------------------------------

class TileSourceUrlArcImagery : public TileSourceUrl {
public:
    TileSourceUrlArcImagery(int request_limit, bool preload);

protected:
    virtual std::string makeUrl(int z, int x, int y) override;

public:
    inline static const char URL_TPL[]{"https://server.arcgisonline.com/ArcGIS/rest/services/"
                                       "World_Imagery/MapServer/tile/$Z$/$Y$/$X$"};

    static std::string makeSourceUrl(int z, int x, int y);
};
using TileSourceUrlArc = TileSourceUrlArcImagery;

#endif // TILESOURCEURLIMPL_H
