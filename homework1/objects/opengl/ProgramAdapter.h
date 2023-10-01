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

    float rotationAngle = 0;
    float scaleX = 1;
    float scaleY = 1;
    float offsetX = 0;
    float offsetY = 0;
    float ratio = 1;
    int width = 0;
    int height = 0;

public:
    ProgramAdapter() {
        GLuint fragmentShader = createFragmentShader();
        GLuint vertexShader = createVertexShader();
        this->id = createProgram(vertexShader, fragmentShader);
    }

    void setScaleX(float scale);

    void setScaleY(float scale);

    void setOffsetX(float offset);

    void setOffsetY(float offset);

    void setRotation(float rotation);

    void setRatio(float ratio);

    void setResolution(int width, int height);

    void useProgram();

    void cleanUniformMatrix4FV(const GLchar *name);

private:
    GLfloat *calcTransformMatrix();
    GLfloat *calcViewMatrixRatio();
    GLfloat *calcViewMatrixResolution();

    void setUniform1F(const GLchar *name, GLfloat value);

    void setUniformMatrix4FV(const GLchar *name, GLfloat *value);
};


#endif //PRACTICE2_PROGRAMADAPTER_H
