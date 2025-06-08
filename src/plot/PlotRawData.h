#ifndef PLOTRAWDATA_H
#define PLOTRAWDATA_H

#include <vector>

/**
 * @class PlotRawData
 * @brief Raw plot data consisting of multiple data points
 */
class PlotRawData {
public:
    void add(float x, float y);
    void clear();
    void eraseOld(size_t count);
    size_t size() const;
    float lastX() const;
    float getXAt(size_t index) const;
    float getYAt(size_t index) const;
    const float* getRawX() const;
    const float* getRawY() const;

private:
    std::vector<float> vx;
    std::vector<float> vy;
};

#endif // PLOTRAWDATA_H
