#include "Isolines.h"

void addMarchingTriangle(std::vector<vertex>& res, const Function& f, int i[3], int j[3], float stepX, float stepY, float z, float t) {
    float x[3];
    float y[3];
    float fx[3];
    for (int iter = 0; iter < 3; ++iter) {
        x[iter] = f.xRange.first + stepX * i[iter];
        y[iter] = f.yRange.first + stepY * j[iter];
        fx[iter] = f(x[iter], y[iter], t);
    }
    for (int iter = 0; iter < 3; ++iter) {
        int iter1 = (iter + 1) % 3;
        if ((fx[iter] - z) * (fx[iter1] - z) < 0) {
            float k = abs(fx[iter] - z) / abs(fx[iter] - fx[iter1]);
            res.push_back(
                    {k * x[iter1] + (1 - k) * x[iter], k * y[iter1] + (1 - k) * y[iter], z,
                     {0, 255, 0}});
        }
    }

}

std::vector<vertex> generateIsolinesVertices(int gridN, int isolinesN, Function f, float t) {
    std::vector<vertex> res;

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

Isolines::Isolines(Function f, int gridN, int isolinesN) : f(std::move(f)), gridN(gridN), isolinesN(isolinesN) {
    vertices = generateIsolinesVertices(gridN, isolinesN, f, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(0); // xy
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)nullptr);
    glEnableVertexAttribArray(1); // z
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, z)));
    glEnableVertexAttribArray(2); // color
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*) offsetof(vertex, color));
}

void Isolines::draw(float t) {
    vertices = generateIsolinesVertices(gridN, isolinesN, f, t);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

void Isolines::updateSettings(const std::map<int, int>& settings) {
    gridN = settings.at(QUALITY_N);
    isolinesN = settings.at(ISOLINES_N);
}