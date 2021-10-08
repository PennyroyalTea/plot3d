#pragma once

#include "memory"
#include "functional"
#include "vector"
#include "iostream"

#include "Graphics.h"

#include "Function.h"

class Mesh {
public:
    virtual void draw(float t) = 0;
};

struct vertex {
    float pos[3];
    std::uint8_t color[3];
};

