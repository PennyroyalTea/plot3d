#include "Grid.h"

std::vector<vertex> generateGridVertices(int gridN) {
    std::vector<vertex> res;
    float step = 0.25f;
    for (int i = 0; i < gridN; ++i) {
        res.push_back({i * step, -gridN * step, 0.f, {255, 255, 255}});
        res.push_back({i * step, gridN * step, 0.f, {255, 255, 255}});
        res.push_back({-i * step, -gridN * step, 0.f, {255, 255, 255}});
        res.push_back({-i * step, gridN * step, 0.f, {255, 255, 255}});

        res.push_back({-gridN * step, i * step, 0.f, {255, 255, 255}});
        res.push_back({gridN * step, i * step, 0.f, {255, 255, 255}});
        res.push_back({-gridN * step, -i * step, 0.f, {255, 255, 255}});
        res.push_back({gridN * step, -i * step, 0.f, {255, 255, 255}});
    }
    return res;
}

Grid::Grid(int gridN) {
    vertices = generateGridVertices(gridN);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // xy
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // z
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) offsetof(vertex, z));
    glEnableVertexAttribArray(2); // color
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*) offsetof(vertex, color));
}

void Grid::draw(float t) {
    if (!doDraw) {
        return;
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

void Grid::updateSettings(const std::map<int, int>& settings) {
    doDraw = settings.at(DRAW_GRID);
}