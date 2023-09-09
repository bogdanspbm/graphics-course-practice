//
// Created by Bogdan Madzhuga on 05.09.2023.
//

#ifndef PRACTICE1_SHADERUTILS_H
#define PRACTICE1_SHADERUTILS_H

#include <GL/glew.h>

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint createShader(GLenum shaderType,
                    const char *shaderSource);


GLuint createFragmentShader();

GLuint createVertexShader();

#endif //PRACTICE1_SHADERUTILS_H
