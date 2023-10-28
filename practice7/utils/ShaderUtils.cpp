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
out vec2 texcoord;

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = normalize(mat3(model) * in_normal);
    fragColor = in_color;
    texcoord = in_texcoord;
}
)";

const char fragmentSource[] =
        R"(#version 330 core
        in vec3 normal;
        in vec2 texcoord;

        uniform sampler2D textureLayer;
        uniform vec3 albedo;
        uniform vec3 ambient_light;
        uniform vec3 sun_color;
        uniform vec3 sun_direction;
        uniform vec3 view_direction;
        uniform float roughness;
        uniform float glossiness;

        layout (location = 0) out vec4 out_color;

vec3 diffuse(vec3 direction) {
    return albedo * max(0.0, dot(normal, direction));
}

vec3 specular(vec3 direction) {
    float power = 1.0 / (roughness * roughness) - 1.0;
	vec3 normalized_view = normalize(view_direction);
	vec3 reflected = reflect(-direction, normal);

    return glossiness * albedo * pow(max(0.0, dot(reflected, normalized_view)), power);
}

void main()
{
    vec3 ambient = albedo * ambient_light;

    vec3 sun_extra = (diffuse(sun_direction) + specular(sun_direction)) * sun_color;

    vec3 color = ambient + sun_extra;

    out_color = vec4(color, 0.5);
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

GLuint createFragmentShader(const char *shaderSource) {
    return createShader(GL_FRAGMENT_SHADER, shaderSource);
}


GLuint createVertexShader() {
    return createShader(GL_VERTEX_SHADER, vertexSource);
}

GLuint createVertexShader(const char *shaderSource) {
    return createShader(GL_VERTEX_SHADER, shaderSource);
}
