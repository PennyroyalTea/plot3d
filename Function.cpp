#include <cmath>

#include "Function.h"

double Functions::Plane::operator()(double x, double y, double t) {
    return 1.0;
}

double Functions::Spikes::operator()(double x, double y, double t) {
    return (x - floor(x)) + (y - floor(y)) + (t - floor(t));
}