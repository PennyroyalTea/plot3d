#include "Grid.h"

std::vector<vertex> generateGridVertices(int gridN) {
    std::vector<vertex> res;
    float step = 0.25f;
    for (int i = 0; i < gridN; ++i) {
        res.push_back({{i * step, -gridN * step, 0.f}, {255, 255, 255}});
        res.push_back({{i * step, gridN * step, 0.f}, {255, 255, 255}});
        res.push_back({{-i * step, -gridN * step, 0.f}, {255, 255, 255}});
        res.push_back({{-i * step, gridN * step, 0.f}, {255, 255, 255}});

        res.push_back({{-gridN * step, i * step, 0.f}, {255, 255, 255}});
        res.push_back({{gridN * step, i * step, 0.f}, {255, 255, 255}});
        res.push_back({{-gridN * step, -i * step, 0.f}, {255, 255, 255}});
        res.push_back({{gridN * step, -i * step, 0.f}, {255, 255, 255}});
    }
    return res;
}

Grid::Grid(int gridN) {
    vertices = generateGridVertices(gridN);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*) offsetof(vertex, color));
}

void Grid::draw(float t) {
    if (!doDraw) {
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

void Grid::updateSettings(const std::map<int, int>& settings) {
    doDraw = settings.at(DRAW_GRID);
}