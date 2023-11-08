
#ifndef HOMEWORK2_GLPROGRAM_H
#define HOMEWORK2_GLPROGRAM_H

#include <map>
#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include "utils/ShaderUtils.h"
#include "glm/vec3.hpp"
#include "enums/GLProgramType.h"
#include "Camera.h"
#include "objects/graphics/light/Sun.h"
#include "objects/graphics/light/Ambient.h"
#include "glm/vec2.hpp"
#include "FrameBuffer.h"

class GLProgram {
private:

    // Program Identification
    static std::map<ProgramType, GLProgram *> programs;
    static GLuint currentProgramID;
    static ProgramType currentProgramType;

    GLuint programID;
    ProgramType type;

    SDL_Window *window;

    FrameBuffer *frameBuffer;

public:
    GLProgram(SDL_Window *window, ProgramType type) {
        this->window = window;
        this->type = type;
        this->camera = new Camera(type);
        programID = createProgram(createVertexShader(type), createFragmentShader(type));
        GLProgram::programs[type] = this;

        if (type == SHADOW) {
            this->frameBuffer = new FrameBuffer(2048, 2048);
        }
    }

    // Camera
    Camera *camera;

public:
    static void createGLPrograms(SDL_Window *window) {
        new GLProgram(window, MAIN);
        new GLProgram(window, SHADOW);
        new GLProgram(window, VIEW);
    }

    static GLProgram *getGLProgram(ProgramType type) {
        if (GLProgram::programs.contains(type)) {
            return GLProgram::programs[type];
        }

        return nullptr;
    }

    static GLProgram *getGLProgram() {
        return getGLProgram(GLProgram::currentProgramType);
    }

private:
    // Methods

    void setViewMatrix();

    void setProjectionMatrix();

public:
    // Methods
    void useProgram() {
        GLProgram::currentProgramType = type;
        GLProgram::currentProgramID = programID;
        glUseProgram(programID);

        SDL_GetWindowSize(window, camera->getWidth(), camera->getHeight());
        glViewport(0, 0, *camera->getWidth(), *camera->getHeight());

        if (frameBuffer != nullptr) {
            frameBuffer->bindFrameBuffer();
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        setViewMatrix();
        setProjectionMatrix();

        camera->bindView();
        Sun::getSun()->bindLight();
        Ambient::getAmbient()->bindLight();
    }

    bool setUniformFloat(const GLchar *name, float value);

    bool setUniformInt(const GLchar *name, int value);

    bool setUniformVector2F(const GLchar *name, glm::vec2 vector);

    bool setUniformVector3F(const GLchar *name, glm::vec3 vector);

    bool setUniformMatrix4FV(const GLchar *name, GLfloat *value, bool transpose);

public:
    // Getters
    Camera *getCamera();

    FrameBuffer *getFrameBuffer();

    GLuint getProgramID();
};


#endif //HOMEWORK2_GLPROGRAM_H
