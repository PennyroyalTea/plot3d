#pragma once

#include "memory"
#include "functional"
#include "vector"
#include "iostream"

#include "Graphics.h"

#include "Function.h"

namespace Mesh {

    class Mesh {
    public:
        virtual void draw(float t) = 0;
    };

    struct surfaceVertex {
        float pos[3];
        std::uint8_t color[3];
    };

    class Surface : public Mesh {
    public:
        Surface(Function f, int gridN);
        void draw(float t) override;
    private:
        Function f;
        int gridN;

        std::vector<surfaceVertex> vertices;
        std::vector<std::uint32_t> verticesOrder;

        GLuint vbo;
        GLuint vao;
        GLuint ebo;
    };
}

