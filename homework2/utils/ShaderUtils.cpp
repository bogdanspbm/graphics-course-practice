#include "ShaderUtils.h"
#include "utils/shaders/MainShaders.h"
#include "utils/shaders/ShadowShader.h"
#include "utils/shaders/ScreenViewShader.h"
#include <stdexcept>
#include <string>

const std::string TAG = "ShaderUtils";


// create_program
GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
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
GLuint createShader(GLenum shaderType,
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


GLuint createFragmentShader(ProgramType type) {
    switch (type) {
        case MAIN:
            return createShader(GL_FRAGMENT_SHADER, mainFragmentSource);
        case SHADOW:
            return createShader(GL_FRAGMENT_SHADER, shadowFragmentSource);
        case VIEW:
            return createShader(GL_FRAGMENT_SHADER, screenViewFragmentSource);
    }
    return createShader(GL_FRAGMENT_SHADER, mainFragmentSource);
}


GLuint createVertexShader(ProgramType type) {
    switch (type) {
        case MAIN:
            return createShader(GL_VERTEX_SHADER, mainVertexSource);
        case SHADOW:
            return createShader(GL_VERTEX_SHADER, shadowVertexSource);
        case VIEW:
            return createShader(GL_VERTEX_SHADER, screenViewVertexSource);
    }
    return createShader(GL_VERTEX_SHADER, mainVertexSource);
}
