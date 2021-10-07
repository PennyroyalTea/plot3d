#include <array>
#include "Mesh.h"

std::vector<Mesh::surfaceVertex> generateVertices(int gridN, const Function& f, float t) {
    std::vector<Mesh::surfaceVertex> res;
    float xStep = (f.xRange.second - f.xRange.first) / gridN;
    float yStep = (f.yRange.second - f.yRange.first) / gridN;
    for (int i = 0; i <= gridN; ++i) {
        for (int j = 0; j <= gridN; ++j) {
            float x = f.xRange.first + i * xStep;
            float y = f.yRange.first + j * yStep;
            float z = f(x, y, t);

            float color0[3] = {0xf0u, 0x48u, 0xc6u};
            float color1[3] = {0xf6u, 0xeau, 0x41u};
            float part = (z - f.zRange.first) / (f.zRange.second - f.zRange.first);
            std::uint8_t color[3];
            for (int i = 0; i < 3; ++i) {
                color[i] = part * color0[i] + (1. - part) * color1[i];
            }

            float xNorm = 2 * (i + .0f) / gridN - 1;
            float yNorm = 2 * (j + .0f) / gridN - 1;
            float zNorm = 2 * (z - f.zRange.first) / (f.zRange.second - f.zRange.first) - 1;

            res.push_back({{xNorm, yNorm, zNorm}, {color[0], color[1], color[2]}});
        }
    }
    return res;
}

std::vector<std::uint32_t> generateVerticesOrder(int gridN) {
    std::vector<std::uint32_t> res;
    for (int i = 0; i < gridN; ++i) {
        for (int j = 0; j < gridN; ++j) {
            res.push_back(i + j * (gridN + 1));
            res.push_back(i + 1 + j * (gridN + 1));
            res.push_back(i + (j + 1) * (gridN + 1));
            res.push_back(i + (j + 1) * (gridN + 1));
            res.push_back(i + 1 + j * (gridN + 1));
            res.push_back(i + 1 + (j + 1) * (gridN + 1));
        }
    }
    return res;
}

Mesh::Surface::Surface(Function f, int gridN): gridN(gridN), f(std::move(f)) {
    vertices = generateVertices(gridN, f, 0);
    verticesOrder = generateVerticesOrder(gridN);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(surfaceVertex), vertices.data(), GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(surfaceVertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(surfaceVertex), (void*) offsetof(surfaceVertex, color));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesOrder.size() * sizeof(std::uint32_t), verticesOrder.data(), GL_DYNAMIC_DRAW);
};


void Mesh::Surface::draw(float t) {
    std::cout << "drawing surface" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, verticesOrder.size(), GL_UNSIGNED_INT, nullptr);
}
