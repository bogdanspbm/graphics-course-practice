#include "ShaderUtils.h"
#include <stdexcept>
#include <string>

const std::string TAG = "ShaderUtils";

const char vertexSource[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in vec3 in_color;

out vec3 fragColor;
out vec3 normal;

void main()
{
   gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = normalize(mat3(model) * in_normal);
    fragColor = in_color;
}
)";

const char fragmentSource[] =
        R"(#version 330 core

in vec3 normal;
in vec3 fragColor; // Receive color from the vertex shader

layout (location = 0) out vec4 out_color;

void main()
{
    vec3 ambient_dir = vec3(0.0, 0.0, -1.0);
    vec3 ambient_color = vec3(0.8);

    vec3 light1_dir = normalize(vec3(0.0, 0.0, -1.0));
    vec3 light2_dir = normalize(vec3(0.0, 0.0, -1.0));

    vec3 light1_color = vec3(1.0, 0.5, 0.25);
    vec3 light2_color = vec3(0.25, 0.5, 1.0);

    vec3 n = normalize(normal);

    vec3 color = (0.5 + 0.5 * dot(n, ambient_dir)) * ambient_color
        + max(0.0, dot(n, light1_dir)) * light1_color
        + max(0.0, dot(n, light2_dir)) * light2_color
        ;

    // Use the color passed from the vertex shader
    color *= fragColor;

    float gamma = 1.0 / 2.2;
    out_color = vec4(fragColor, 1.0);
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
