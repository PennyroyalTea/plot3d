#include "Surface.h"

std::vector<point2D> generateSurfaceGrid(int gridN, const Function& f) {
    std::vector<point2D> res;
    for (int i = 0; i <= gridN; ++i) {
        for (int j = 0; j <= gridN; ++j) {
            float xNorm = 2 * (i + .0f) / gridN - 1;
            float yNorm = 2 * (j + .0f) / gridN - 1;
            res.push_back({xNorm, yNorm});
        }
    }
    return res;
}

std::vector<float> generateSurfaceValues(const std::vector<point2D>& grid, const Function& f, float t) {
    std::vector<float> res;
    for (const auto& [x, y] : grid) {
        res.push_back(f(x, y, t));
    }
    return res;
}

std::vector<color> generateSurfaceColors(const std::vector<point2D>& grid, const Function& f, float t) {
    std::vector<color> res;

    uint8_t color0[3] = {0xf0u, 0x48u, 0xc6u};
    uint8_t color1[3] = {0xf6u, 0xeau, 0x41u};
    uint8_t color[3];
    for (const auto& [x, y] : grid) {
        float z = f(x, y, t);
        float part = (z - f.zRange.first) / (f.zRange.second - f.zRange.first);
        for (int i = 0; i < 3; ++i) {
            color[i] = part * color0[i] + (1. - part) * color1[i];
        }
        res.push_back({color[0], color[1], color[2]});
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

Surface::Surface(Function f, int gridN): gridN(gridN), f(std::move(f)) {
    verticesGrid = generateSurfaceGrid(gridN, f);
    verticesValues = generateSurfaceValues(verticesGrid, f, 0.f);
    verticesColors = generateSurfaceColors(verticesGrid, f, 0.f);
    verticesOrder = generateSurfaceVerticesOrder(gridN);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesOrder.size() * sizeof(std::uint32_t), verticesOrder.data(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &vboGrid);
    glBindBuffer(GL_ARRAY_BUFFER, vboGrid);
    glBufferData(GL_ARRAY_BUFFER, verticesGrid.size() * sizeof(point2D), verticesGrid.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0); // position xy
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &vboValues);
    glBindBuffer(GL_ARRAY_BUFFER, vboValues);
    glBufferData(GL_ARRAY_BUFFER, verticesValues.size() * sizeof(float), verticesValues.data(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(1); // position z
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, verticesColors.size() * sizeof(color), verticesColors.data(), GL_STREAM_DRAW);
    glEnableVertexAttribArray(2); // color
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);
};


void Surface::draw(float t) {
    verticesValues = generateSurfaceValues(verticesGrid, f, t);
    verticesColors = generateSurfaceColors(verticesGrid, f, t);

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vboValues);
    glBufferData(GL_ARRAY_BUFFER, verticesValues.size() * sizeof(float), verticesValues.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, verticesColors.size() * sizeof(color), verticesColors.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, verticesOrder.size(), GL_UNSIGNED_INT, nullptr);
}

void Surface::updateSettings(const std::map<int, int>& settings) {
    if (settings.at(QUALITY_N) != gridN) {
        gridN = settings.at(QUALITY_N);
        verticesGrid = generateSurfaceGrid(gridN, f);
        verticesOrder = generateSurfaceVerticesOrder(gridN);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesOrder.size() * sizeof(std::uint32_t), verticesOrder.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vboGrid);
        glBufferData(GL_ARRAY_BUFFER, verticesGrid.size() * sizeof(point2D), verticesGrid.data(), GL_DYNAMIC_DRAW);
    }
    if (settings.at(DRAW_FACES)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}
