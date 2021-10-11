#pragma once

#include "Mesh.h"

class Surface : public Mesh {
public:
    Surface(Function f, int gridN);
    void draw(float t) override;
    void updateSettings(const std::map<int, int>& settings) override;
private:
    Function f;
    int gridN;

    std::vector<point2D> verticesGrid;
    std::vector<float> verticesValues;
    std::vector<color> verticesColors;

    std::vector<std::uint32_t> verticesOrder;

    GLuint vboGrid, vboValues, vboColors;
    GLuint vao;
    GLuint ebo;
};
