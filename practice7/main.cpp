#ifdef WIN32
#include <SDL.h>
#undef main
#else

#include <SDL2/SDL.h>

#endif

#include <GL/glew.h>

#include <string_view>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include "objects/opengl/ProgramAdapter.h"
#include "objects/graphics/Placeable.h"


std::string to_string(std::string_view str) {
    return std::string(str.begin(), str.end());
}

void sdl2_fail(std::string_view message) {
    throw std::runtime_error(to_string(message) + SDL_GetError());
}

void glew_fail(std::string_view message, GLenum error) {
    throw std::runtime_error(to_string(message) + reinterpret_cast<const char *>(glewGetErrorString(error)));
}


int main() try {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        sdl2_fail("SDL_Init: ");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 7",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (!window)
        sdl2_fail("SDL_CreateWindow: ");

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
        sdl2_fail("SDL_GL_CreateContext: ");

    if (auto result = glewInit(); result != GLEW_NO_ERROR)
        glew_fail("glewInit: ", result);

    if (!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 is not supported");

    glClearColor(0.8f, 0.8f, 1.f, 0.f);

    auto program = new ProgramAdapter();
    program->addPointLight(PointLight{Vector3F(0.5, 0, -1), Vector3F(0.5, 0.7, 0.1), Vector3F(1, 0, 0.1)});
    program->addPointLight(PointLight{Vector3F(-0.5, 0.3, -2), Vector3F(0.8, 0.2, 0.3), Vector3F(1, 0, 0.1)});
    program->addPointLight(PointLight{Vector3F(0, -0.3, -0.5), Vector3F(0.1, 0.1, 0.9), Vector3F(1, 0, 0.1)});

    std::string project_root = PROJECT_ROOT;
    std::string suzanne_model_path = project_root + "/suzanne.obj";
    auto suzanne = new Placeable(program, suzanne_model_path);
    auto texture = new Texture();
    suzanne->addTexture(texture);
    suzanne->setPosition({0, 0, -1.5});
    suzanne->setScale({0.5f, 0.5f, 0.5f});
    suzanne->setRotation({0, 45, 0});

    auto last_frame_start = std::chrono::high_resolution_clock::now();

    float time = 0.f;

    std::map<SDL_Keycode, bool> button_down;

    bool transparent = false;

    float camera_distance = 3.f;
    float camera_x = 0.f;
    float camera_angle = 0.f;

    bool running = true;
    while (running) {
        for (SDL_Event event; SDL_PollEvent(&event);)
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            width = event.window.data1;
                            height = event.window.data2;
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    button_down[event.key.keysym.sym] = true;
                    if (event.key.keysym.sym == SDLK_SPACE)
                        transparent = !transparent;
                    break;
                case SDL_KEYUP:
                    button_down[event.key.keysym.sym] = false;
                    break;
            }

        if (!running)
            break;

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();
        last_frame_start = now;
        time += dt;

        if (button_down[SDLK_UP])
            camera_distance -= 4.f * dt;
        if (button_down[SDLK_DOWN])
            camera_distance += 4.f * dt;

        if (button_down[SDLK_LEFT])
            camera_angle += 2.f * dt;
        if (button_down[SDLK_RIGHT])
            camera_angle -= 2.f * dt;

        if (button_down[SDLK_KP_4])
            camera_x -= 4.f * dt;
        if (button_down[SDLK_KP_6])
            camera_x += 4.f * dt;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 1.f, 0.f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);


        program->useProgram();
        suzanne->draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
