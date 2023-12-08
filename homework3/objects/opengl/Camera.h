//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_CAMERA_H
#define HOMEWORK2_CAMERA_H


#include "glm/vec3.hpp"
#include "enums/GLProgramType.h"
#include "objects/input/KeyHandler.h"
#include "glm/fwd.hpp"

class Camera {

private:

    bool DEBUG = false;

    glm::vec3 location = {0, 0, -10.0};
    glm::vec3 rotation = {0, 0, 0};

    int width = 1920;
    int height = 1080;

    // Projection Params
    float near = 10.f;
    float far = 4000.0f;
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

    glm::mat4 calcViewMatrix();

    glm::mat4 calcProjectionMatrix();

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
