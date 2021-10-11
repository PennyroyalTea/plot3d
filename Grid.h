#pragma once

#include "Mesh.h"

class Grid : public Mesh {
public:
    Grid(int gridN);
    void draw(float t) override;
    void updateSettings(const std::map<int, int>& settings) override;
    void updateFunction(const Function& f) override {};
private:
    bool doDraw;
    std::vector<vertex> vertices;

    GLuint vbo;
    GLuint vao;
};