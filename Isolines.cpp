#include "Isolines.h"

void addMarchingTriangle(
        std::vector<vertex>& vertices,
        std::vector<uint32_t>& order,
        std::map<std::pair<std::pair<int, int>, int>, int>& id_map, // {edge, isolineId} -> orderId
        int isolineId,
        const Function& f,
        int i[3], int j[3], int id[3],
        float stepX, float stepY,
        float z, float t) {

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
        if ((fx[iter] - z) * (fx[iter1] - z) > 0) {
            continue;
        }

        std::pair<int, int> edge = {id[iter], id[iter1]};
        std::pair<int, int> edge_back = {id[iter1], id[iter]};

        if (id_map.count({edge, isolineId})) {
            order.push_back(id_map[{edge, isolineId}]);
        } else if (id_map.count({edge_back, isolineId})) {
            order.push_back(id_map[{edge_back, isolineId}]);
        } else {
            id_map[{edge, isolineId}] = vertices.size();
            float xNorm1 = 2 * ((x[iter1] - f.xRange.first) / (f.xRange.second - f.xRange.first)) - 1.f;
            float xNorm = 2 * ((x[iter] - f.xRange.first) / (f.xRange.second - f.xRange.first)) - 1.f;
            float yNorm1 = 2 * ((y[iter1] - f.yRange.first) / (f.yRange.second - f.yRange.first)) - 1.f;
            float yNorm = 2 * ((y[iter] - f.yRange.first) / (f.yRange.second - f.yRange.first)) - 1.f;
            float zNorm = 2 * ((z - f.zRange.first) / (f.zRange.second - f.zRange.first)) - 1.f;
            float k = abs(fx[iter] - z) / abs(fx[iter] - fx[iter1]);
            vertices.push_back(
                    {k * xNorm1 + (1 - k) * xNorm, k * yNorm1 + (1 - k) * yNorm, zNorm,
                     {0, 255, 0}});
            order.push_back(id_map[{edge, isolineId}]);
        }
    }

}

std::pair<std::vector<vertex>, std::vector<uint32_t>> generateIsolines(int gridN, int isolinesN, Function f, float t) {
    std::vector<vertex> vertices_res;
    std::vector<uint32_t> order_res;
    std::map<std::pair<std::pair<int, int>, int>, int> id_map;

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
                int id1[3] = {i * gridN + j, i * gridN + j + 1, (i + 1) * gridN + j};
                int id2[3] = {i * gridN + j + 1, (i + 1) * gridN + j, (i + 1) * gridN + (j + 1)};
                addMarchingTriangle(vertices_res, order_res, id_map, isolineId, f, i1, j1, id1, stepX, stepY, z, t);
                addMarchingTriangle(vertices_res, order_res, id_map, isolineId, f, i2, j2, id2, stepX, stepY, z, t);
            }
        }
    }
    return {vertices_res, order_res};
}

Isolines::Isolines(Function f, int gridN, int isolinesN) : f(std::move(f)), gridN(gridN), isolinesN(isolinesN) {
    auto [vertices, order] = generateIsolines(gridN, isolinesN, f, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(std::uint32_t), order.data(), GL_DYNAMIC_DRAW);


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
    auto [vertices, order] = generateIsolines(gridN, isolinesN, f, t);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(std::uint32_t), order.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_LINES, order.size(), GL_UNSIGNED_INT, nullptr);
}

void Isolines::updateSettings(const std::map<int, int>& settings) {
    gridN = settings.at(QUALITY_N);
    isolinesN = settings.at(ISOLINES_N);
}

void Isolines::updateFunction(const Function& f) {
    this -> f = f;
}