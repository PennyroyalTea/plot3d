#pragma once

#include "Mesh.h"

class Grid : public Mesh {
public:
    Grid(int gridN);
    void draw(float t) override;
private:
    std::vector<vertex> vertices;

    GLuint vbo;
    GLuint vao;
};