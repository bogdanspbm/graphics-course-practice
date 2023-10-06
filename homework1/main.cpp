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
#include "math.h"
#include "objects/input/KeyHandler.h"
#include "objects/graphics/Renderable.h"
#include "objects/graphics/Landscape.h"
#include "objects/opengl/ProgramAdapter.h"
#include "objects/graphics/Isolines.h"

std::string to_string(std::string_view str) {
    return std::string(str.begin(), str.end());
}

void sdl2_fail(std::string_view message) {
    throw std::runtime_error(to_string(message) + SDL_GetError());
}

void glew_fail(std::string_view message, GLenum error) {
    throw std::runtime_error(to_string(message) + reinterpret_cast<const char *>(glewGetErrorString(error)));
}


GLuint create_shader(GLenum type, const char *source) {
    GLuint result = glCreateShader(type);
    glShaderSource(result, 1, &source, nullptr);
    glCompileShader(result);
    GLint status;
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint info_log_length;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, '\0');
        glGetShaderInfoLog(result, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Shader compilation failed: " + info_log);
    }
    return result;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint result = glCreateProgram();
    glAttachShader(result, vertex_shader);
    glAttachShader(result, fragment_shader);
    glLinkProgram(result);

    GLint status;
    glGetProgramiv(result, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLint info_log_length;
        glGetProgramiv(result, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, '\0');
        glGetProgramInfoLog(result, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Program linkage failed: " + info_log);
    }

    return result;
}

int main() try {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        sdl2_fail("SDL_Init: ");

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

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 4",
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

    glClearColor(0.1f, 0.1f, 0.2f, 0.f);

    auto *program = new ProgramAdapter();

    std::string project_root = PROJECT_ROOT;
    
    float time = 0.f;
    float scale = 0.5f;

    auto last_frame_start = std::chrono::high_resolution_clock::now();
    auto *keyHandler = new KeyHandler();
    auto landscapeA = new Landscape(program, [&time](float x, float y) -> float {
        return 0;
    });

    auto isolinesA = new Isolines(landscapeA, program);

    /*auto landscapeB = new Landscape(program, [&time](float x, float y) -> float {
        return 0;
    });


    auto landscapeC = new Landscape(program, [&time](float x, float y) -> float {
        return 0;
    });*/

    landscapeA->setPosition({-0.75, 0.35, -0.75});
    isolinesA->setPosition({-0.75, -0.35, -0.75});
/*
    landscapeB->setFunctionScale(0.05);
    landscapeB->setPosition({0, 0, -0.75});
    landscapeB->setColors({0.32, 0.72, 0.1}, {0.98, 0.18, 0.59});

    landscapeC->setPosition({0.75, 0, -0.75});
    landscapeC->setColors({0.21, 0.81, 0.79}, {0.18, 0.33, 0.92});
*/

    float dt = 0;
    float speed = 1;

    keyHandler->bindOnPressedEvent([&dt, program, &speed]() -> void {
        Vector3D rightVector = program->getRightVector();
        program->addPosition(rightVector * -dt * speed);
    }, SDL_KeyCode::SDLK_LEFT);

    keyHandler->bindOnPressedEvent([&dt, program, &speed]() -> void {
        Vector3D rightVector = program->getRightVector();
        program->addPosition(rightVector * dt * speed);
    }, SDL_KeyCode::SDLK_RIGHT);

    keyHandler->bindOnPressedEvent([&dt, program, &speed]() -> void {
        Vector3D forwardVector = program->getForwardVector();
        program->addPosition(forwardVector * dt * speed);
    }, SDL_KeyCode::SDLK_DOWN);

    keyHandler->bindOnPressedEvent([&dt, &program, &speed]() -> void {
        Vector3D forwardVector = program->getForwardVector();
        program->addPosition(forwardVector * -dt * speed);
    }, SDL_KeyCode::SDLK_UP);


    std::map<SDL_Keycode, bool> button_down;

    glEnable(GL_DEPTH_TEST);

    bool running = true;
    while (running) {

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        auto now = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();


        for (SDL_Event event; SDL_PollEvent(&event);) {
            keyHandler->handleKeyboardEvent(event.key);
            keyHandler->handleMouseEvent(event.button);
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
            }
        }

        if (!running)
            break;

        last_frame_start = now;
        time += dt;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program->useProgram();
        program->setProjectionMatrix();
        program->setViewMatrix();

        landscapeA->updateFunction([&time](float x, float y) -> float {
            return cos(24 * sqrt(x * x + y * y) - 2 * time);
        });

        isolinesA->updateFunction();
/*
        landscapeB->updateFunction([&time](float x, float y) -> float {
            return sin(M_PI * 2 * x - sin(1.8 * time)) * cos(M_PI * 2 * y + cos(1.8 * time));
        });

        landscapeC->updateFunction([&time](float x, float y) -> float {
            return pow((x * x - y * y) / (x * x + y * y), 2) * cos(6 * M_PI * x * y - 1.8 * time);
        });*/

        landscapeA->setScale({scale, scale, scale * 0.05f});
        landscapeA->draw();

        isolinesA->setScale({scale, scale, scale * 0.05f});
        isolinesA->draw();

        /* landscapeB->setScale({scale, scale, scale * 0.05f});
         landscapeB->draw();

         landscapeC->setScale({scale, scale, scale * 0.05f});
         landscapeC->draw();*/

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}