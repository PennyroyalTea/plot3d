#pragma once

#include "Mesh.h"

class Surface : public Mesh {
public:
    Surface(Function f, int gridN);
    void draw(float t) override;
private:
    Function f;
    int gridN;

    std::vector<vertex> vertices;
    std::vector<std::uint32_t> verticesOrder;

    GLuint vbo;
    GLuint vao;
    GLuint ebo;
};
