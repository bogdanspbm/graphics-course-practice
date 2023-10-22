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

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/scalar_constants.hpp>
#include "objects/graphics/Placeable.h"
#include "objects/graphics/basic/Plane.h"
#include "objects/opengl/ScreenView.h"


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

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 6",
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

    std::string project_root = PROJECT_ROOT;
    std::string cow_model = project_root + "/cow.obj";
    std::string cow_texture_path = project_root + "/cow.png";
    auto texture = new Texture(cow_texture_path);

    auto program = new ProgramAdapter();
    program->setFrameBuffer(width, height);


    auto screenView = new ScreenView(Vector2F{-0.5f, -0.5f}, Vector2F{0.5f, 0.5f});

    screenView->setTexture(program->getTexture());
    //screenView->setTexture(texture);

    auto cow = new Placeable(program, cow_model);
    cow->addTexture(texture);
    cow->setPosition({0, 0, -1.5});
    cow->setScale({0.75f, 0.75f, 0.75f});
    cow->setRotation({0, 135, 0});

    auto last_frame_start = std::chrono::high_resolution_clock::now();

    float time = 0.f;

    std::map<SDL_Keycode, bool> button_down;

    float camera_distance = 0.5f;
    float model_angle = glm::pi<float>() / 2.f;

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
                            glViewport(0, 0, width, height);
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    button_down[event.key.keysym.sym] = true;
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
            camera_distance -= 1.f * dt;
        if (button_down[SDLK_DOWN])
            camera_distance += 1.f * dt;

        if (button_down[SDLK_LEFT])
            model_angle -= 2.f * dt;
        if (button_down[SDLK_RIGHT])
            model_angle += 2.f * dt;


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program->setClearColor(Vector4F{1, 0, 0, 1});
        cow->setRotation({-90, 0, 0});
        program->useProgram();
        cow->draw();
        screenView->setPosition(Vector2F{-0.5f, -0.5f});
        screenView->draw(window);


        program->setClearColor(Vector4F{0, 1, 0, 1});
        cow->setRotation({0, 135, 0});
        program->useProgram();
        cow->draw();
        screenView->setPosition(Vector2F{0.5f, -0.5f});
        screenView->draw(window);

        program->setClearColor(Vector4F{0, 0, 0, 1});
        cow->setRotation({0, 90, 0});
        program->useProgram();
        cow->draw();
        screenView->setPosition(Vector2F{-0.5f, 0.5f});
        screenView->draw(window);


        program->setClearColor(Vector4F{0, 0, 1, 1});
        cow->setRotation({0,180,0});
        program->useProgram();
        cow->draw();
        screenView->setPosition(Vector2F{0.5f, 0.5f});
        screenView->draw(window);


        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
