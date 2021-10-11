#pragma once

#include "Mesh.h"

class Isolines : public Mesh {
public:
    Isolines(Function f, int gridN, int isolinesN);
    void draw(float t) override;
    void updateSettings(const std::map<int, int>& settings) override;
    void updateFunction(const Function& f) override;
private:
    Function f;

    int gridN;
    int isolinesN;

    std::vector<vertex> vertices;

    GLuint vbo;
    GLuint vao;
    GLuint ebo;
};
