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

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include "objects/graphics/renderable/Placeable.h"
#include "objects/graphics/renderable/ScreenView.h"
#include "objects/graphics/volumes/CubeVolume.h"
#include "utils/ObjectUtils.h"

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

    SDL_Window *window = SDL_CreateWindow("Graphics course practice 9",
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


    GLProgram::createGLPrograms(window);
    auto screenView = new ScreenView(GLProgram::getGLProgram(SHADOW)->getFrameBuffer()->getTexture());

    auto keyHandler = new KeyHandler();
    GLProgram::getGLProgram(LIGHT)->getCamera()->bindControl(keyHandler);
    GLProgram::getGLProgram(VOLUMETRIC)->getCamera()->bindControl(keyHandler);

    Ambient::getAmbient()->setColor({0.5, 0.5, 0.5});
    Sun::getSun()->setColor({0.8, 0.72, 0.74});
    Sun::getSun()->setDirection({0, 1, 0});

    auto fog = new CubeVolume();
    fog->getPlaceable()->setScale({10, 10, 10});

    std::string project_root = PROJECT_ROOT;
    std::string scene_path = project_root + "/sphere.obj";

    auto renderList = loadRenderableListFromFile(project_root + "/sponza/sponza.obj");
    auto materialList = loadMaterialListFromFile(project_root + "/sponza/sponza.mtl");
    std::vector<Placeable *> objectList;

    for (auto renderable: renderList) {
        objectList.push_back(new Placeable(renderable));
    }


    std::string rabitPath = project_root + "/bunny.obj";
    auto bunny = new Placeable(rabitPath);
    bunny->setRotation({0, 45, 0});
    bunny->setPosition({0, 1, 0});

    auto sphere = new Placeable(scene_path);
    sphere->setRotation({150, 0, 0});
    sphere->setScale({35,35,35});
    sphere->setPosition({0, 1, 0});

    sphere->getMaterial()->addTexture(Texture::getTexture(project_root + "/textures/brick_albedo.jpg", DEFAULT));
    sphere->getMaterial()->addTexture(Texture::getTexture(project_root + "/textures/brick_ao.jpg", GLOSS_MAP));
    sphere->getMaterial()->addTexture(Texture::getTexture(project_root + "/textures/brick_roughness.jpg", ROUGH_MAP));
    sphere->getMaterial()->addTexture(
            Texture::getTexture(project_root + "/textures/environment_map_reflection.jpg", REFLECTION_MAP));


    auto cubeMap = new Placeable(project_root + "/cube_map.obj");
    cubeMap->setScale({2000,2000,2000});
    cubeMap->setRotation({180,0,0});
    cubeMap->getMaterial()->addTexture(Texture::getTexture(project_root + "/textures/environment_map.jpg", DEFAULT));

    auto last_frame_start = std::chrono::high_resolution_clock::now();

    float time = 0.f;
    bool paused = false;

    std::map<SDL_Keycode, bool> button_down;

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
                            glViewport(0, 0, width, height);
                            break;
                    }
                    break;
            }
            keyHandler->handleInputEvent(event);
        }

        if (!running)
            break;



        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();
        last_frame_start = now;
        if (!paused)
            time += dt;

        Sun::getSun()->setDirection({std::cos(time * 0.25f), 4, std::sin(time * 0.25f)});

        GLProgram::getGLProgram(SHADOW)->useProgram();
        sphere->draw();
        for (int i = 0; i < objectList.size(); i++) {
            objectList[i]->draw();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.8f, 0.8f, 0.9f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLProgram::getGLProgram(LIGHT)->useProgram();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        sphere->draw();
        cubeMap->draw();

        for (int i = 0; i < objectList.size(); i++) {
            objectList[i]->draw();
        }


        //glDisable(GL_BLEND);

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