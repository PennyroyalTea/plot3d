#include "Scene.h"

const char vertex_shader_source[] =
        R"(#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
out vec4 color;
void main()
{
	gl_Position = vec4(in_position, 1.0);
	color = in_color;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core
in vec4 color;
layout (location = 0) out vec4 out_color;
void main()
{
	out_color = color;
}
)";

GLuint create_shader(GLenum type, const char * source)
{
    GLuint result = glCreateShader(type);
    glShaderSource(result, 1, &source, nullptr);
    glCompileShader(result);
    GLint status;
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint info_log_length;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, '\0');
        glGetShaderInfoLog(result, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Shader compilation failed: " + info_log);
    }
    return result;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint result = glCreateProgram();
    glAttachShader(result, vertex_shader);
    glAttachShader(result, fragment_shader);
    glLinkProgram(result);

    GLint status;
    glGetProgramiv(result, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint info_log_length;
        glGetProgramiv(result, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, '\0');
        glGetProgramInfoLog(result, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Program linkage failed: " + info_log);
    }

    return result;
}

void sdl2_fail(const std::string& message) {
    throw std::runtime_error(message + SDL_GetError());
}

void glew_fail(const std::string& message, GLenum error) {
    throw std::runtime_error(message + reinterpret_cast<const char *>(glewGetErrorString(error)));
}

void initSDL(SDL_GLContext& glContext, SDL_Window*& window, int& width, int& height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl2_fail("SDL_Init: ");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("3D plot builder",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           800, 600,
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (!window) {
        sdl2_fail("SDL_CreateWindow: ");
    }

    SDL_GetWindowSize(window, &width, &height);

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        sdl2_fail("SDL_GL_CreateContext: ");
    }

    if (auto result = glewInit(); result != GLEW_NO_ERROR) {
        glew_fail("glewInit: ", result);
    }

    if (!GLEW_VERSION_3_3) {
        throw std::runtime_error("OpenGL 3.3 is not supported");
    }
}

Scene::Scene() {
    initSDL(glContext, window, width, height);

    glClearColor(0.8f, 0.8f, 1.f, 0.f);

    auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
    auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    program = create_program(vertex_shader, fragment_shader);

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    lastFrameStart = std::chrono::high_resolution_clock::now();
    currentTime = 0.f;
}

Scene::~Scene() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}

void Scene::addObject(std::unique_ptr<Mesh::Mesh> mesh) {
    meshes.push_back(std::move(mesh));
}

void Scene::drawingLoop() {

    bool running = true;
    while (running) {
        for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT: switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            width = event.window.data1;
                            height = event.window.data2;
                            glViewport(0, 0, width, height);
                            break;
                    }
                    break;
        }
        if (!running) {
            break;
        }
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastFrameStart).count();
        lastFrameStart = now;
        currentTime += dt;

        glClear(GL_COLOR_BUFFER_BIT);
//        glClear(GL_DEPTH_BUFFER_BIT);



        glUseProgram(program);

        for (std::unique_ptr<Mesh::Mesh>& mesh : meshes) {
            mesh -> draw(currentTime);
        }

        SDL_GL_SwapWindow(window);
    }
}