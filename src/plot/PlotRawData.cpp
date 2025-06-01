#include "PlotRawData.h"

void PlotRawData::add(float x, float y) {
    vx.push_back(x);
    vy.push_back(y);
}

void PlotRawData::clear() {
    vx.clear();
    vy.clear();
}

void PlotRawData::eraseOld(size_t count) {
    vx.erase(vx.begin(), vx.begin() + count);
    vy.erase(vy.begin(), vy.begin() + count);
}

size_t PlotRawData::size() const {
    return vx.size();
}

float PlotRawData::lastX() const {
    return vx.back();
}

float PlotRawData::getXAt(size_t index) const {
    return vx[index];
}

float PlotRawData::getYAt(size_t index) const {
    return vy[index];
}

const float* PlotRawData::getRawX() const {
    return vx.data();
}

const float* PlotRawData::getRawY() const {
    return vy.data();
}
