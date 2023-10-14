//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_PROGRAMADAPTER_H
#define PRACTICE2_PROGRAMADAPTER_H


#include <GL/glew.h>
#include <string>
#include "utils/ShaderUtils.h"
#include "structures/Vector3D.h"

class ProgramAdapter {
private:
    // Program Params
    const std::string TAG = "ProgramAdapter";
    GLuint id;

    // Camera Transform
    Vector3D position = Vector3D();
    Vector3D rotation = Vector3D();

    // Resolution Params
    float width = 1920;
    float height = 1080;

    // Projection Params
    float near = 0.001f;
    float far = 1000.0f;
    float fov = 90;

public:
    ProgramAdapter() {
        GLuint fragmentShader = createFragmentShader();
        GLuint vertexShader = createVertexShader();
        this->id = createProgram(vertexShader, fragmentShader);
    }

    void useProgram();

    void cleanUniformMatrix4FV(const GLchar *name);

    void setUniformMatrix4FV(const GLchar *name, GLfloat *value);

    void setProjectionMatrix();

    void setViewMatrix();

    void setResolution(float width, float height);

    void setPosition(Vector3D position);

    void addPosition(Vector3D offset);

    void setRotation(Vector3D position);

    void addRotation(Vector3D offset);

    void setFOV(float fov);

    Vector3D getForwardVector();

    Vector3D getRightVector();
protected:
    void calcProjectionMatrix(float matrix[16]);

    void calcViewMatrix(float matrix[16]);
};


#endif //PRACTICE2_PROGRAMADAPTER_H
