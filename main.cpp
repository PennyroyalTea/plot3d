#include <iostream>

#include "Function.h"
#include "Mesh.h"
#include "Scene.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Scene scene;

    Function f(
            [](float x, float y, float t) {return x;},
            {-1, 1}, {-1, 1}, {-1, 1});

    std::unique_ptr<Mesh::Mesh> surface = std::make_unique<Mesh::Surface>(f, 3);

    scene.addObject(std::move(surface));

    scene.drawingLoop();
    return 0;
}
