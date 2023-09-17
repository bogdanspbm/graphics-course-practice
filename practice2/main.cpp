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
#include "objects/ProgramAdapter.h"
#include "objects/KeyHandler.h"

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

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 1",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (!window)
        sdl2_fail("SDL_CreateWindow: ");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
        sdl2_fail("SDL_GL_CreateContext: ");

    if (auto result = glewInit(); result != GLEW_NO_ERROR)
        glew_fail("glewInit: ", result);

    if (!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 is not supported");

    glClearColor(0.8f, 0.8f, 1.f, 0.f);

    auto *program = new ProgramAdapter();
    auto *keyHandler = new KeyHandler();

    float posX = 0, posY = 0;
    float speed = 0.5f;

    GLuint vao;
    glGenVertexArrays(1, &vao);

    int startTick = 0;
    clock_t lastTick = clock();

    bool running = true;
    while (running) {

        const auto deltaTick = clock() - lastTick;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        for (SDL_Event event; SDL_PollEvent(&event);) {
            keyHandler->handleKeyboardEvent(event.key);
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        if (!running)
            break;

        glClear(GL_COLOR_BUFFER_BIT);

        if (keyHandler->isPressed(SDL_KeyCode::SDLK_LEFT)) {
            posX -= speed * (float) deltaTick / CLOCKS_PER_SEC;
        }

        if (keyHandler->isPressed(SDL_KeyCode::SDLK_RIGHT)) {
            posX += speed * (float) deltaTick / CLOCKS_PER_SEC;
        }

        if (keyHandler->isPressed(SDL_KeyCode::SDLK_UP)) {
            posY += speed * (float) deltaTick / CLOCKS_PER_SEC;
        }

        if (keyHandler->isPressed(SDL_KeyCode::SDLK_DOWN)) {
            posY -= speed * (float) deltaTick / CLOCKS_PER_SEC;
        }

        program->setScaleX(0.2);
        program->setScaleY(0.2);
        program->setOffsetX(posX);
        program->setOffsetY(posY);
        program->setRatio((float) width / (float) height);
        program->setRotation((float) lastTick / CLOCKS_PER_SEC);
        program->useProgram();


        // Draw Triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);


        SDL_GL_SwapWindow(window);
        lastTick += deltaTick;
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
