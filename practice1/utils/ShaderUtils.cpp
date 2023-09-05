#include "ShaderUtils.h"
#include <stdexcept>
#include <string>


GLuint create_shader(GLenum shader_type,
                     const char *shader_source) {

    // Create Shader
    GLuint shaderID = glCreateShader(shader_type);
    const GLchar *shadersArray[1] = {shader_source};
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
            throw std::invalid_argument("Shader Compile: Error!\nShader Compile: Can't read log.");
        }

        std::string log(logLen, '\0');
        glGetShaderInfoLog(shaderID, logLen, NULL, &log[0]);
        throw std::invalid_argument("Shader Compile: Error!\nShader Compile Log: " + log);
    }

    return shaderID;
}