#include <array>
#include "Mesh.h"

std::vector<Mesh::vertex> generateSurfaceVertices(int gridN, const Function& f, float t) {
    std::vector<Mesh::vertex> res;
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

std::vector<std::uint32_t> generateSurfaceVerticesOrder(int gridN) {
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
    vertices = generateSurfaceVertices(gridN, f, 0);
    verticesOrder = generateSurfaceVerticesOrder(gridN);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*) offsetof(vertex, color));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesOrder.size() * sizeof(std::uint32_t), verticesOrder.data(), GL_DYNAMIC_DRAW);
};


void Mesh::Surface::draw(float t) {
    std::cout << "drawing surface" << std::endl;
    vertices = generateSurfaceVertices(gridN, f, t);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, verticesOrder.size(), GL_UNSIGNED_INT, nullptr);
}

std::vector<Mesh::vertex> generateGridVertices(int gridN) {
    std::vector<Mesh::vertex> res;
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

Mesh::Grid::Grid(int gridN) {
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

void Mesh::Grid::draw(float t) {
    std::cout << "drawing grid" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

void addMarchingTriangle(std::vector<Mesh::vertex>& res, const Function& f, int i[3], int j[3], float stepX, float stepY, float z, float t) {
    float x[3];
    float y[3];
    float fx[3];
    for (int iter = 0; iter < 3; ++iter) {
        x[iter] = f.xRange.first + stepX * i[iter];
        y[iter] = f.yRange.first + stepY * j[iter];
        fx[iter] = f(x[iter], y[iter], t);
    }
    std::vector<Mesh::vertex> tmp;
    for (int iter = 0; iter < 3; ++iter) {
        int iter1 = (iter + 1) % 3;
        if ((fx[iter] - z) * (fx[iter1] - z) < 0) {
            tmp.push_back({{x[iter], y[iter], fx[iter]}, {255, 255, 0}});
            tmp.push_back({{x[iter1], y[iter1], fx[iter1]}, {255, 255, 0}});

            float k = abs(fx[iter] - z) / abs(fx[iter] - fx[iter1]);
            std::cout << "k = " << k << "add isopoint: " << k * x[iter] + (1 - k) * x[iter1] << " " << k * y[iter] + (1 - k) * y[iter1] << " " << z << std::endl;
            res.push_back(
                    {{k * x[iter1] + (1 - k) * x[iter], k * y[iter1] + (1 - k) * y[iter], z},
                     {0, 255, 0}});
        }
    }

    for (const auto& v : tmp){
        res.push_back(v);
    }

}

std::vector<Mesh::vertex> generateIsolinesVertices(int gridN, int isolinesN, Function f, float t) {
    std::vector<Mesh::vertex> res;

    float stepX = (f.xRange.second - f.xRange.first) / gridN;
    float stepY = (f.yRange.second - f.yRange.first) / gridN;

    float stepZ = (f.zRange.second - f.zRange.first) / (isolinesN + 1);

    for (int isolineId = 0; isolineId < isolinesN; ++isolineId) {
        for (int i = 0; i < gridN; ++i) {
            for (int j = 0; j < gridN; ++j) {
                float z = f.zRange.first + stepZ * (isolineId + 1);

                int i1[3] = {i, i, i + 1};
                int j1[3] = {j, j + 1, j};
                int i2[3] = {i, i + 1, i + 1};
                int j2[3] = {j + 1, j, j + 1};
                addMarchingTriangle(res, f, i1, j1, stepX, stepY, z, t);
                addMarchingTriangle(res, f, i2, j2, stepX, stepY, z, t);
            }
        }
    }
    return res;
}

Mesh::Isolines::Isolines(Function f, int gridN, int isolinesN) : f(std::move(f)), gridN(gridN), isolinesN(isolinesN) {
    vertices = generateIsolinesVertices(gridN, isolinesN, f, 0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*) offsetof(vertex, color));
}

void Mesh::Isolines::draw(float t) {
    std::cout << "drawing isolines" << std::endl;
    vertices = generateIsolinesVertices(gridN, isolinesN, f, t);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
}
