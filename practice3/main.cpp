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
#include "structures/Vertex.h"
#include "utils/BezierUtils.h"

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

    std::vector<Vertex> points;
    std::vector<Vertex> bezierPoints;
    std::vector<Vector> bezierColors;

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    int quality = 20;

    GLuint bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    GLuint bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    GLuint colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, bezierColors.size() * sizeof(Vector), bezierColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (0));
    glEnableVertexAttribArray(1);

    auto updatePointFunction = [&bezierPoints, &bezierColors, &colorVBO, &points, &VBO, &bezierVBO, &quality]() -> void {
        bezierPoints = computeBezierCurve(points, quality);
        bezierColors = computeBezierColors(bezierPoints);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
        glBufferData(GL_ARRAY_BUFFER, bezierPoints.size() * sizeof(Vertex), bezierPoints.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, bezierColors.size() * sizeof(Vector), bezierColors.data(),
                     GL_STATIC_DRAW);
    };

    keyHandler->bindOnMouseClickEvent(
            [&points, &updatePointFunction](Position position) -> void {
                Vertex vertex = Vertex{(float) position.x, (float) position.y, 0};
                points.push_back(vertex);
                updatePointFunction();
            }, SDL_BUTTON_LEFT);

    keyHandler->bindOnMouseClickEvent([&points, &updatePointFunction](Position position) -> void {
        points.pop_back();
        updatePointFunction();
    }, SDL_BUTTON_RIGHT);

    keyHandler->bindOnPressEvent([&quality, &updatePointFunction]() -> void {
        quality -= 1;
        if (quality < 1) {
            quality = 1;
        }
        updatePointFunction();
    }, SDL_KeyCode::SDLK_LEFT);

    keyHandler->bindOnPressEvent([&quality, &updatePointFunction]() -> void {
        quality += 1;
        updatePointFunction();
    }, SDL_KeyCode::SDLK_RIGHT);


    int startTick = 0;
    clock_t lastTick = clock();

    bool running = true;
    while (running) {

        const auto deltaTick = clock() - lastTick;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);


        for (SDL_Event event; SDL_PollEvent(&event);) {
            keyHandler->handleKeyboardEvent(event.key);
            keyHandler->handleMouseEvent(event.button);
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }


        if (!running)
            break;

        glClear(GL_COLOR_BUFFER_BIT);

        program->setResolution(width, height);
        program->useProgram();

        glPointSize(3);

        glBindVertexArray(VBO);
        glDrawArrays(GL_LINE_STRIP, 0, points.size());
        glDrawArrays(GL_POINTS, 0, points.size());

        glBindVertexArray(bezierVBO);
        glDrawArrays(GL_LINE_STRIP, 0, bezierPoints.size());

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
