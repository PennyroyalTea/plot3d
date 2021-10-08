#include <iostream>

#include "Function.h"
#include "Scene.h"
#include "Mesh.h"
#include "Surface.h"
#include "Grid.h"
#include "Isolines.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Scene scene;
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    Function f(
            [](float x, float y, float t) {return cos(x) * sin(y) * sin(t);},
            {-1, 1}, {-1, 1}, {-1, 1});

    std::unique_ptr<Mesh> surface = std::make_unique<Surface>(f, 3);
    std::unique_ptr<Mesh> isoline = std::make_unique<Isolines>(f, 3, 3);
//    std::unique_ptr<Mesh> grid = std::make_unique<Grid>(15);

    scene.addObject(std::move(surface));
    scene.addObject(std::move(isoline));
//    scene.addObject(std::move(grid));

    scene.drawingLoop();
    return 0;
}
