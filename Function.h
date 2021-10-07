#pragma once

#include "functional"
#include "utility"

class Function {
public:
    Function(
            std::function<float(float, float, float)> f,
            std::pair<float, float> xRange, std::pair<float, float> yRange, std::pair<float, float> zRange);
    float operator()(float x, float y, float t) const;
private:
    std::function<float(float, float, float)> f;
public:
    std::pair<float, float> xRange, yRange, zRange;
};

