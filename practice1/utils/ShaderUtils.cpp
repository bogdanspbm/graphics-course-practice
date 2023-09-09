#include "ShaderUtils.h"
#include <stdexcept>
#include <string>

const std::string TAG = "ShaderUtils";

const char fragmentSource[] =
        R"(#version 330 core
layout (location = 0)

out vec3 outColor;
in vec2 texCoord;

void main() {
    int x = int(texCoord.x * 16.0);
    int y = int(texCoord.y * 16.0);
    bool isBlack = (x + y) % 2 == 0;
    outColor = isBlack ? vec3(0.0, 0.0, 0.0) : vec3(1.0, 1.0, 1.0);
}
)";

const char vertexSource[] =
        R"(#version 330 core
const vec2 VERTICES[3] = vec2[3](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0)
);

out vec2 texCoord;

void main()
{
    gl_Position = vec4(VERTICES[gl_VertexID], 0.0, 1.0);
    texCoord = VERTICES[gl_VertexID];
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
