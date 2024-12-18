#ifndef ITILELOADER_H
#define ITILELOADER_H

using ImTextureID = void*;

class ITileLoader {
public:
    virtual ~ITileLoader() = default;

    virtual void beginLoad(int z, int xmin, int xmax, int ymin, int ymax) {
    }
    virtual ImTextureID tileAt(int z, int x, int y) = 0;
    virtual int tileCount() const {
        return 0;
    };
    virtual void endLoad() {
    }
};

#endif // ITILELOADER_H
