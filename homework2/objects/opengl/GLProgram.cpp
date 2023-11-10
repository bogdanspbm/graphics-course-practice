//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "GLProgram.h"

std::map<ProgramType, GLProgram *> GLProgram::programs;
GLuint GLProgram::currentProgramID;
ProgramType GLProgram::currentProgramType;

Camera *GLProgram::getCamera() {
    return this->camera;
}

bool GLProgram::setUniformMatrix4FV(const GLchar *name, GLfloat *value, bool transpose) {
    GLint uniformLocationID = glGetUniformLocation(programID, name);

    if (uniformLocationID == -1) {
        return false;
    }

    glUniformMatrix4fv(uniformLocationID, 1, transpose ? GL_TRUE : GL_FALSE, value);
    return true;
}

bool GLProgram::setUniformVector2F(const GLchar *name, glm::vec2 vector) {
    GLint uniformLocationID = glGetUniformLocation(programID, name);

    if (uniformLocationID == -1) {
        return false;
    }

    glUniform2f(uniformLocationID, vector.x, vector.y);
    return true;
}

bool GLProgram::setUniformVector3F(const GLchar *name, glm::vec3 vector) {
    GLint uniformLocationID = glGetUniformLocation(programID, name);

    if (uniformLocationID == -1) {
        return false;
    }

    glUniform3f(uniformLocationID, vector.x, vector.y, vector.z);
    return true;
}

bool GLProgram::setUniformFloat(const GLchar *name, float value) {
    GLint uniformLocationID = glGetUniformLocation(programID, name);

    if (uniformLocationID == -1) {
        return false;
    }

    glUniform1f(uniformLocationID, value);
    return true;
}

void GLProgram::setViewMatrix() {
    float view[16];
    camera->calcViewMatrix(view);
    setUniformMatrix4FV("view", view, true);
}

void GLProgram::setProjectionMatrix() {
    float projection[16];
    camera->calcProjectionMatrix(projection);
    setUniformMatrix4FV("projection", projection, true);
}

GLuint GLProgram::getProgramID() {
    return programID;
}

bool GLProgram::setUniformInt(const GLchar *name, int value) {
    GLint uniformLocationID = glGetUniformLocation(programID, name);

    if (uniformLocationID == -1) {
        return false;
    }

    glUniform1i(uniformLocationID, value);
    return true;
}

FrameBuffer *GLProgram::getFrameBuffer() {
    return frameBuffer;
}

void GLProgram::setSunViewMatrix() {
    float view[16];
    getGLProgram(SHADOW)->getCamera()->calcViewMatrix(view);
    setUniformMatrix4FV("sunView", view, true);
}

