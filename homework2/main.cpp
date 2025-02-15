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
#include <vector>
#include <map>
#include <cmath>


#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>
#include "objects/opengl/GLProgram.h"
#include "objects/graphics/renderable/Placeable.h"
#include "objects/graphics/renderable/ScreenView.h"
#include "utils/ObjectUtils.h"
#include "objects/input/KeyHandler.h"
#include "objects/opengl/LightRender.h"


std::string to_string(std::string_view str) {
    return std::string(str.begin(), str.end());
}

void sdl2_fail(std::string_view message) {
    throw std::runtime_error(to_string(message) + SDL_GetError());
}

void glew_fail(std::string_view message, GLenum error) {
    throw std::runtime_error(to_string(message) + reinterpret_cast<const char *>(glewGetErrorString(error)));
}

int main()
try {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        sdl2_fail("SDL_Init: ");

    std::string project_root = PROJECT_ROOT;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 8",
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

    GLProgram::createGLPrograms(window);

    auto keyHandler = new KeyHandler();
    GLProgram::getGLProgram(MAIN)->getCamera()->bindControl(keyHandler);
    GLProgram::getGLProgram(LIGHT)->getCamera()->bindControl(keyHandler);

    auto screenView = new ScreenView(GLProgram::getGLProgram(SHADOW)->getFrameBuffer()->getTexture());

    Ambient::getAmbient()->setColor({1, 1, 1});
    Sun::getSun()->setColor({0.2, 0.12, 0.14});
    Sun::getSun()->setDirection({0, 10, -1});

    auto greenLight = DirectionLight{{0, 150, -400}};
    greenLight.rotation = {0,180,0};
    greenLight.color = {0,1,0};
    greenLight.range = 300;
    LightRender::getLightRenderer()->addDirectionLight(greenLight);

    auto blueLight = DirectionLight{{0, 150, 400}};
    blueLight.rotation = {0,0,0};
    blueLight.useRotation = true;
    blueLight.color = {0,0,1};
    blueLight.range = 300;
    LightRender::getLightRenderer()->addDirectionLight(blueLight);


    auto renderList = loadRenderableListFromFile(project_root + "/sponza/sponza.obj");
    auto materialList = loadMaterialListFromFile(project_root + "/sponza/sponza.mtl");
    std::vector<Placeable *> objectList;

    for (auto renderable: renderList) {
        objectList.push_back(new Placeable(renderable));
    }

    auto cowObject = new Placeable(project_root + "/models/cow.obj");
    cowObject->getMaterial()->addTexture(Texture::getTexture(project_root + "/models/cow.png", DEFAULT));
    cowObject->getMaterial()->addTexture(Texture::getTexture(project_root + "/models/cow_normal.png", NORMAL_MAP));
    cowObject->setPosition({0, 0, -1});
    cowObject->setScale({0.25, 0.25, 0.25});
    cowObject->setRotation({0, 135, 0});

    auto last_frame_start = std::chrono::high_resolution_clock::now();

    float time = 0.f;

    bool running = true;
    while (running) {
        for (SDL_Event event; SDL_PollEvent(&event);) {
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
            }
            keyHandler->handleInputEvent(event);
        }

        if (!running)
            break;

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();
        last_frame_start = now;
        time += dt;

        glViewport(0, 0, width, height);
        glClearColor(7.f, 0.55f, 0.5f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable alpha testing
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5f);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        LightRender::getLightRenderer()->renderLight(objectList);

         GLProgram::getGLProgram(MAIN)->useProgram();
         for (int i = 0; i < objectList.size(); i++) {
             objectList[i]->draw();
         }


        glClearColor(0.8f, 0.8f, 1.f, 0.f);

        screenView->draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
