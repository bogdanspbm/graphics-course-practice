//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_PROGRAMADAPTER_H
#define PRACTICE2_PROGRAMADAPTER_H


#include <GL/glew.h>
#include "utils/ShaderUtils.h"
#include "structures/Vector3F.h"
#include "objects/graphics/Texture.h"
#include "FrameBuffer.h"
#include <string>
#include <SDL2/SDL_video.h>
#include "structures/PointLight.h"

class ProgramAdapter {
private:
    // Program Params
    const std::string TAG = "ProgramAdapter";
    FrameBuffer *frameBuffer;
    GLuint id;

    ProgramAdapter* lightAdapter;

    // Light
    Vector3F ambientLight = Vector3F(0.1f, 0.1f, 0.1f);
    Vector3F sunLightColor = Vector3F(0.8f, 0.5f, 0.8f);
    Vector3F sunDirection = Vector3F(0.4, 0.6, 0.4);

    std::vector<PointLight> lightPoints;

    // Camera Transform
    Vector3F position = Vector3F();
    Vector3F rotation = Vector3F();

    // Resolution Params
    float width = 1920;
    float height = 1080;

    // Projection Params
    float near = 0.001f;
    float far = 5.0f;
    float fov = 90;

public:
    ProgramAdapter() {
        GLuint fragmentShader = createFragmentShader();
        GLuint vertexShader = createVertexShader();
        this->id = createProgram(vertexShader, fragmentShader);
        setViewMatrix();
        setProjectionMatrix();
    }

    ProgramAdapter(GLuint fragmentShader, GLuint vertexShader) {
        this->id = createProgram(vertexShader, fragmentShader);
        setViewMatrix();
        setProjectionMatrix();
    }

    void setAmbientLight(Vector3F light);

    void setSunLight(Vector3F color, Vector3F direction);

    void setUniformVector3F(const GLchar *name, Vector3F vector);

    void setUniformFloat(const GLchar *name, float value);

    void setLight();

    void setFrameBuffer(FrameBuffer *buffer);

    void setFrameBuffer(int width, int height);

    void useProgram(SDL_Window *window);

    void cleanUniformMatrix4FV(const GLchar *name);

    void setUniformMatrix4FV(const GLchar *name, GLfloat *value, bool transpose);

    void setProjectionMatrix();

    void setViewMatrix();

    void setResolution(float width, float height);

    void setPosition(Vector3F position);

    void addPosition(Vector3F offset);

    void setRotation(Vector3F rotation);

    void addRotation(Vector3F rotation);

    void setFOV(float fov);

    void bindTexture(Texture *texture);

    Vector3F getForwardVector();

    Vector3F getRightVector();

    void setClearColor(Vector4F clearColor);

    Texture *getTexture();

    void addPointLight(PointLight point);

    void bindPointLights();

    void setLightAdapter(ProgramAdapter* adapter);


    void calcProjectionMatrix(float matrix[16]);

    void calcViewMatrix(float matrix[16]);
};


#endif //PRACTICE2_PROGRAMADAPTER_H
