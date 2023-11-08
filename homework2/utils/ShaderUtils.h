//
// Created by Bogdan Madzhuga on 05.09.2023.
//

#ifndef HOMEWORK2_SHADERUTILS_H
#define HOMEWORK2_SHADERUTILS_H

#include <GL/glew.h>
#include "enums/GLProgramType.h"

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint createShader(GLenum shaderType,
                    const char *shaderSource);


GLuint createFragmentShader(ProgramType type);

GLuint createVertexShader(ProgramType type);


#endif //HOMEWORK2_SHADERUTILS_H
