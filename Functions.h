#pragma once

#include "Function.h"

Function f_cossinsin(
        [](float x, float y, float t) {return cos(x) * sin(y) * sin(t);},
        {-10, 10}, {-10, 10}, {-1, 1});

Function f_metaballs(
        [](float x, float y, float t) {
            float xi[2] = {cos(t) / 2, .3f};
            float yi[2] = {sin(t) / 2, -.5f};
            float ri[2] = {.5f, .35f};
            float ci[2] = {1.f, -.75f};
            float res = 0;
            for (int i = 0; i < 2; ++i) {
                res += ci[i] * exp(-((x - xi[i]) * (x - xi[i]) + (y - yi[i]) * (y - yi[i])) / (ri[i] * ri[i]));
            }
            return res;
            },
        {-1, 1}, {-1, 1}, {-1, 1}
        );