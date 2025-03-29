#ifndef TILESOURCEURL_H
#define TILESOURCEURL_H

#include "TileSourceAsync.h"

class TileSourceUrl : public TileSourceAsync {
public:
    TileSourceUrl(int requestLimit, bool preload, const std::string& userAgent = "curl");
    virtual ~TileSourceUrl();

protected:
    virtual bool receiveTile(int z, int x, int y, TileData& tileData) override;
    virtual std::string makeUrl(int z, int x, int y) = 0;

private:
    std::string _userAgent{"curl"};
};

#endif // TILESOURCEURL_H
