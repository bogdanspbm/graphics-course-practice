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
#include "objects/Model.h"
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

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = normalize(mat3(model) * in_normal);
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

in vec3 normal;

layout (location = 0) out vec4 out_color;

void main()
{
    vec3 ambient_dir = vec3(0.0, 1.0, 0.0);
    vec3 ambient_color = vec3(0.2);

    vec3 light1_dir = normalize(vec3( 3.0, 2.0,  1.0));
    vec3 light2_dir = normalize(vec3(-3.0, 2.0, -1.0));

    vec3 light1_color = vec3(1.0,  0.5, 0.25);
    vec3 light2_color = vec3(0.25, 0.5, 1.0 );

    vec3 n = normalize(normal);

    vec3 color = (0.5 + 0.5 * dot(n, ambient_dir)) * ambient_color
        + max(0.0, dot(n, light1_dir)) * light1_color
        + max(0.0, dot(n, light2_dir)) * light2_color
        ;

    float gamma = 1.0 / 2.2;
    out_color = vec4(pow(min(vec3(1.0), color), vec3(gamma)), 1.0);
}
)";

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

    auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
    auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    auto program = create_program(vertex_shader, fragment_shader);

    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");

    std::string project_root = PROJECT_ROOT;
    Model bunny = Model(project_root + "/bunny.obj");

    auto last_frame_start = std::chrono::high_resolution_clock::now();
    auto *keyHandler = new KeyHandler();

    float dt = 0;
    float x = 0;
    float y = 0;

    float speed = 10;

    keyHandler->bindOnPressedEvent([&dt, &x, &speed]() -> void {
        x -= 1 * dt * speed;
    }, SDL_KeyCode::SDLK_LEFT);

    keyHandler->bindOnPressedEvent([&dt, &x, &speed]() -> void {
        x += 1 * dt * speed;
    }, SDL_KeyCode::SDLK_RIGHT);

    keyHandler->bindOnPressedEvent([&dt, &y, &speed]() -> void {
        y -= 1 * dt * speed;
    }, SDL_KeyCode::SDLK_DOWN);

    keyHandler->bindOnPressedEvent([&dt, &y, &speed]() -> void {
        y += 1 * dt * speed;
    }, SDL_KeyCode::SDLK_UP);

    float time = 0.f;

    std::map<SDL_Keycode, bool> button_down;

    float scale = 0.5f;
    float near = 0.001f;
    float far = 1000.0f;
    float fov = 90;
    float right = near * tan(fov / 2.0f); // fov - угол обзора в радианах

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


        float aspectRatio = width / height;
        float top = right / aspectRatio;

        float model[16] =
                {
                        scale * cos(time), scale * -sin(time), 0.0f, x,
                        scale * sin(time), scale * cos(time), 0.0f, y,
                        0.0f, 0.0f, scale, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                };

        float view[16] =
                {
                        1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, -2.f,
                        0.f, 0.f, 0.f, 1.f,
                };

        float projection[16] =
                {
                        (2.0f * near) / (right * 2.0f), 0.0f, 0.0f, 0.0f,
                        0.0f, (2.0f * near) / (top * 2.0f), 0.0f, 0.0f,
                        0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near),
                        0.0f, 0.0f, -1.0f, 0.0f
                };

        glUseProgram(program);
        glUniformMatrix4fv(model_location, 1, GL_TRUE, model);
        glUniformMatrix4fv(view_location, 1, GL_TRUE, view);
        glUniformMatrix4fv(projection_location, 1, GL_TRUE, projection);
        bunny.draw();

        model[3] -= 1.f;
        model[7] -= 1.f;
        model[8] = model[4];
        model[2] = model[1];
        model[10] = model[5];
        model[1] = 0;
        model[4] = 0;
        model[5] = scale;
        glUniformMatrix4fv(model_location, 1, GL_TRUE, model);
        bunny.draw();

        model[3] += 2.f;
        model[7] += 2.f;
        model[9] = model[2];
        model[6] = model[8];
        model[5] = model[0];
        model[8] = 0;
        model[2] = 0;
        model[0] = scale;
        glUniformMatrix4fv(model_location, 1, GL_TRUE, model);
        bunny.draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}