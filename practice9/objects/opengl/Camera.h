//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_CAMERA_H
#define HOMEWORK2_CAMERA_H


#include "glm/vec3.hpp"
#include "enums/GLProgramType.h"
#include "objects/input/KeyHandler.h"

class Camera {

private:

    bool DEBUG = false;

    glm::vec3 location = {0, 100, 0};
    glm::vec3 rotation = {0, 180, 0};

    int width = 1920;
    int height = 1080;

    // Projection Params
    float near = 1.f;
    float far = 2000.0f;
    float fov = 90;

    // Program Type
    ProgramType type;

public:
    Camera(ProgramType type);

    const glm::vec3 &getLocation() const;

    float getFov();

    float getFar();

    float getNear();

    void setLocation(const glm::vec3 &location);

    const glm::vec3 &getRotation() const;

    void setRotation(const glm::vec3 &rotation);

    void calcViewMatrix(float *matrix);

    void calcProjectionMatrix(float *matrix);

    void bindView();

    void bindControl(KeyHandler* keyHandler);

    int *getWidth() {
        return &width;
    }

    int *getHeight() {
        return &height;
    }
};


#endif //HOMEWORK2_CAMERA_H
