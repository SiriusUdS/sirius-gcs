#ifndef RICHMARKSTORAGE_H
#define RICHMARKSTORAGE_H

#include "Coords.h"

#include <ini.h>
#include <memory>
#include <vector>

class IRichItem;
class MarkItem;

class MarkStorage {
    friend class MarkEditorWidget;

public:
    MarkStorage();
    ~MarkStorage();

    std::shared_ptr<MarkItem> findMark(const std::string& name) const;
    GeoCoords findMark(const std::string& name, bool& ok) const;

    void loadState(const mINI::INIStructure& ini);
    void saveState(mINI::INIStructure& ini) const;

    inline void setPickCoords(const GeoCoords& coords) {
        _pickCoords = coords;
        _pickState = true;
    }

    void addMark(const GeoCoords& coords, const std::string& name);
    void rmMarks();

    struct ItemNode;
    inline std::vector<ItemNode>& markItems() {
        return _markItems;
    }
    inline const std::vector<ItemNode>& markItems() const {
        return _markItems;
    }

private:
    bool handlePickState();
    bool handleLoadState();

private:
    // Load State
    bool _loadState{false};

    // Pick State
    GeoCoords _pickCoords{};
    bool _pickState{false};

public:
    struct ItemNode {
        std::shared_ptr<MarkItem> ptr;
        mutable bool rmFlag{false};
    };

private:
    // Mark Items
    std::vector<ItemNode> _markItems;
};

#endif // RICHMARKITEM_H
