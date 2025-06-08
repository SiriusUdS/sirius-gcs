#include "PlotRawData.h"

/**
 * @brief Add a data point
 * @param x The x value of the data point
 * @param y The y value of the data point
 */
void PlotRawData::add(float x, float y) {
    vx.push_back(x);
    vy.push_back(y);
}

/**
 * @brief Clear all data points
 */
void PlotRawData::clear() {
    vx.clear();
    vy.clear();
}

/**
 * @brief Clear a portion of the oldest data points
 * @param count Number of data points to delete
 */
void PlotRawData::eraseOld(size_t count) {
    vx.erase(vx.begin(), vx.begin() + count);
    vy.erase(vy.begin(), vy.begin() + count);
}

/**
 * @brief Number of data points
 * @returns Data point amount
 */
size_t PlotRawData::size() const {
    return vx.size();
}

/**
 * @brief The x value of the latest data point
 * @returns Last x value
 */
float PlotRawData::lastX() const {
    return vx.back();
}

/**
 * @brief Returns the x value of a specific data point
 * @param index Index of the data point to get the x value from
 * @returns The data point's x value
 */
float PlotRawData::getXAt(size_t index) const {
    return vx[index];
}

/**
 * @brief Returns the y value of a specific data point
 * @param index Index of the data point to get the y value from
 * @returns The data point's y value
 */
float PlotRawData::getYAt(size_t index) const {
    return vy[index];
}

/**
 * @brief Returns the pointer of the data points' x values
 * @returns Pointer of the raw x values
 */
const float* PlotRawData::getRawX() const {
    return vx.data();
}

/**
 * @brief Returns the pointer of the data points' y values
 * @returns Pointer of the raw y values
 */
const float* PlotRawData::getRawY() const {
    return vy.data();
}
