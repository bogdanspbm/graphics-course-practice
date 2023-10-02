//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_PROGRAMADAPTER_H
#define PRACTICE2_PROGRAMADAPTER_H


#include <GL/glew.h>
#include <string>
#include "utils/ShaderUtils.h"

class ProgramAdapter {
private:
    const std::string TAG = "ProgramAdapter";
    GLuint id;

public:
    ProgramAdapter() {
        GLuint fragmentShader = createFragmentShader();
        GLuint vertexShader = createVertexShader();
        this->id = createProgram(vertexShader, fragmentShader);
    }

    void useProgram();

    void cleanUniformMatrix4FV(const GLchar *name);

    void setUniformMatrix4FV(const GLchar *name, GLfloat *value);
};


#endif //PRACTICE2_PROGRAMADAPTER_H
