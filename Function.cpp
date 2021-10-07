#include "Function.h"

#include <utility>

Function::Function(std::function<float(float, float, float)> f,
                   std::pair<float, float> xRange,
                   std::pair<float, float> yRange,
                   std::pair<float, float> zRange) :
                    f(std::move(f)),
                    xRange(std::move(xRange)),
                    yRange(std::move(yRange)),
                    zRange(std::move(zRange)) {}

float Function::operator()(float x, float y, float t) const {
    return f(x, y, t);
};
