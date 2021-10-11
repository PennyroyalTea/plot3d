#pragma once

#include "memory"
#include "functional"
#include "vector"
#include "map"
#include "iostream"

#include "Graphics.h"
#include "Function.h"

const int DRAW_GRID = 0x00; // settings[DRAW_GRID] = 0/1 does grid need to be drawn
const int DRAW_FACES = 0x01; // settings[DRAW_FACES] = 0/1 do surface triangle faces need to be drawn
const int ISOLINES_N = 0x02; // settings[ISOLINES_N] = n # of isolines
const int QUALITY_N = 0x03; // settings[QUALITY_N] = n # of surface grid cells

struct vertex {
    float x;
    float y;
    float z;
    uint8_t color[3];
};

class Mesh {
public:
    virtual void draw(float t) = 0;
    virtual void updateSettings(const std::map<int, int>& settings) = 0;
};

struct point2D {
    float x;
    float y;
};

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
