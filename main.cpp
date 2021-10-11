#include <iostream>

#include "Function.h"
#include "Scene.h"
#include "Mesh.h"
#include "Surface.h"
#include "Grid.h"
#include "Isolines.h"

#include "Functions.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Scene scene(f_cossinsin);

    scene.addFunction(f_metaballs);
//    scene.addFunction(f_perlin);

    std::unique_ptr<Mesh> surface = std::make_unique<Surface>(f_cossinsin, 3);
    std::unique_ptr<Mesh> isoline = std::make_unique<Isolines>(f_cossinsin, 3, 3);
    std::unique_ptr<Mesh> grid = std::make_unique<Grid>(15);

    scene.addObject(std::move(surface));
    scene.addObject(std::move(isoline));
    scene.addObject(std::move(grid));

    scene.drawingLoop();
    return 0;
}
