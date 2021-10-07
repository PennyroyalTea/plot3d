#include "Graphics.h"
#include "Mesh.h"

#include "cmath"
#include "string"
#include "map"
#include "chrono"
#include "vector"
#include "memory"

class Scene {
public:
    Scene();
    ~Scene();
    void addObject(std::unique_ptr<Mesh::Mesh> mesh);
    void drawingLoop();
private:
    SDL_GLContext glContext;
    SDL_Window * window;

    int width;
    int height;

    std::vector<std::unique_ptr<Mesh::Mesh>> meshes;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameStart;
    float currentTime;

    GLuint program;
    GLuint transformLocation;
    GLuint viewLocation;
};