//
// Created by Bogdan Madzhuga on 05.09.2023.
//

#ifndef HOMEWORK2_SHADERUTILS_H
#define HOMEWORK2_SHADERUTILS_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "enums/GLProgramType.h"
#include "utils/shaders/ScreenViewShader.h"

inline std::string projectRoot = PROJECT_ROOT;
inline const std::string TAG = "ShaderUtils";


// create_program
inline GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
    // Create Program
    GLuint programID = glCreateProgram();

    // Attach Shaders
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    // Link Program
    glLinkProgram(programID);

    // Get Status
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

    if (status == GL_TRUE) {
        return programID;
    }

    // Process Error

    GLint logLen;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);

    if (logLen <= 0) {
        glDeleteProgram(programID);
        throw std::invalid_argument(TAG + ": Can't read log.");
    }

    std::string log(logLen, '\0');
    glGetProgramInfoLog(programID, logLen, nullptr, &log[0]);
    glDeleteProgram(programID);

    throw std::invalid_argument(TAG + ":\n" + log);
}

// create_shader
inline GLuint createShader(GLenum shaderType,
                           const char *shaderSource) {

    // Create Shader
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar *shadersArray[1] = {shaderSource};
    glShaderSource(shaderID, 1, shadersArray, nullptr);

    // Compile Shader
    glCompileShader(shaderID);

    // Get Shader compile status
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

    // Process Error
    if (compileStatus != GL_TRUE) {

        GLint logLen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen <= 0) {
            throw std::invalid_argument(TAG + ": Can't read log.");
        }

        std::string log(logLen, '\0');
        glGetShaderInfoLog(shaderID, logLen, nullptr, &log[0]);
        throw std::invalid_argument(TAG + ":\n" + log);
    }

    return shaderID;
}

inline std::string readShaderFile(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();

    std::string output = buffer.str();

    return output;
}


inline GLuint createFragmentShader(ProgramType type) {
    switch (type) {
        case MAIN:
            return createShader(GL_FRAGMENT_SHADER,
                                readShaderFile(projectRoot + "/utils/shaders/main_shader.frag").c_str());
        case LIGHT:
            return createShader(GL_FRAGMENT_SHADER,
                                readShaderFile(projectRoot + "/utils/shaders/light_shader.frag").c_str());
        case SHADOW:
            return createShader(GL_FRAGMENT_SHADER,  readShaderFile(projectRoot + "/utils/shaders/shadow_shader_depth.frag").c_str());
        case VIEW:
            return createShader(GL_FRAGMENT_SHADER, screenViewFragmentSource);
    }
    return createShader(GL_FRAGMENT_SHADER, readShaderFile(projectRoot + "/utils/shaders/main_shader.frag").c_str());
}


inline GLuint createVertexShader(ProgramType type) {
    switch (type) {
        case MAIN:
            return createShader(GL_VERTEX_SHADER,
                                readShaderFile(projectRoot + "/utils/shaders/main_shader.vert").c_str());
        case LIGHT:
            return createShader(GL_VERTEX_SHADER,
                                readShaderFile(projectRoot + "/utils/shaders/main_shader.vert").c_str());
        case SHADOW:
            return createShader(GL_VERTEX_SHADER,  readShaderFile(projectRoot + "/utils/shaders/shadow_shader_depth.vert").c_str());
        case VIEW:
            return createShader(GL_VERTEX_SHADER, screenViewVertexSource);
    }
    return createShader(GL_VERTEX_SHADER, readShaderFile(projectRoot + "/utils/shaders/main_shader.vert").c_str());
}


#endif //HOMEWORK2_SHADERUTILS_H
