#include "Graphics.h"
#include "Mesh.h"

#include "string"
#include "chrono"

class Scene {
public:
    Scene();
    ~Scene();
    void addObject(const Mesh::Mesh& mesh);
    void drawingLoop();
private:
    SDL_GLContext glContext;
    SDL_Window * window;

    int width;
    int height;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameStart;
    float currentTime;

    GLuint program;

//    temp
    GLuint vao;
};