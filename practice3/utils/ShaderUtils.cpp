#include "ShaderUtils.h"
#include <stdexcept>
#include <string>

const std::string TAG = "ShaderUtils";

const char fragmentSource[] =
        R"(#version 330 core

in vec4 color;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = color;
}
)";

const char vertexSource[] =
        R"(#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;

uniform mat4 transform = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);

uniform mat4 view = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);

out vec4 color;

void main()
{
    vec4 transformedPosition = transform * vec4(inPosition, 1.0);
    gl_Position = view * transformedPosition;
    color = inColor;
}
)";

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

GLuint createFragmentShader() {
    return createShader(GL_FRAGMENT_SHADER, fragmentSource);
}

GLuint createVertexShader() {
    return createShader(GL_VERTEX_SHADER, vertexSource);
}
