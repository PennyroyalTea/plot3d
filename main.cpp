#include <iostream>

#include "Function.h"
#include "Mesh.h"
#include "Scene.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Scene scene;

    Function f(
            [](float x, float y, float t) {return 2 * (x * x + y * y) / 2 - 1;},
            {-1, 1}, {-1, 1}, {-1, 1});

    std::unique_ptr<Mesh::Mesh> surface = std::make_unique<Mesh::Surface>(f, 50);
    std::unique_ptr<Mesh::Mesh> grid = std::make_unique<Mesh::Grid>(30);

    scene.addObject(std::move(surface));
    scene.addObject(std::move(grid));

    scene.drawingLoop();
    return 0;
}
