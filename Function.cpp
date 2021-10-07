#include "Function.h"

Function::Function(std::function<float(float, float, float)> f,
                   std::pair<float, float> xRange,
                   std::pair<float, float> yRange,
                   std::pair<float, float> zRange) : f(f), xRange(xRange), yRange(yRange), zRange(zRange) {}

float Function::operator()(float x, float y, float t) const {
    return f(x, y, t);
};
