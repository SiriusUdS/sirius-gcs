#ifndef TILEASYNC_H
#define TILEASYNC_H

#include <chrono>
#include <future>
#include <memory>

class ITile;
using namespace std::chrono_literals;

struct TileAsync {
    struct FutureData {
        std::shared_ptr<ITile> tile;
    };

    int z{}, x{}, y{};
    std::future<FutureData> future;

    bool ready() const {
        return future.wait_for(0s) == std::future_status::ready;
    }

    bool operator==(const TileAsync& other) const {
        return this->z == other.z && this->x == other.x && this->y == other.y;
    }
};

#endif // TILEASYNC_H
