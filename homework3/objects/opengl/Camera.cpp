//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include <cstring>
#include <iostream>
#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "objects/graphics/light/Sun.h"
#include "GLProgram.h"
#include "utils/MathUtils.h"
#include "LightRender.h"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(ProgramType type) {
    this->type = type;
}

const glm::vec3 &Camera::getLocation() const {
    if (type == SHADOW || DEBUG) {
        return LightRender::getLightRenderer()->getLocation();
    }

    return location;
}

void Camera::setLocation(const glm::vec3 &location) {
    Camera::location = location;
}

const glm::vec3 &Camera::getRotation() const {
    if (type == SHADOW || DEBUG) {
        return LightRender::getLightRenderer()->getRotation();
    }

    return rotation;
}

void Camera::setRotation(const glm::vec3 &rotation) {
    Camera::rotation = rotation;
}


glm::mat4 Camera::calcViewMatrix() {
    glm::mat4 view(1.f);
    view = glm::rotate(view,  glm::radians(this->rotation.x), {1.f, 0.f, 0.f});
    view = glm::rotate(view,  glm::radians(this->rotation.y), {0.f, 1.f, 0.f});
    view = glm::translate(view, this->location);
    return view;
}


glm::mat4 Camera::calcProjectionMatrix() {
    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::pi<float>() / 2.f, (1.f * width) / height, near, far);
    return projection;
}

void Camera::bindView() {
    GLProgram::getGLProgram()->setUniformFloat("far", getFar());
    GLProgram::getGLProgram()->setUniformFloat("near", getNear());
    GLProgram::getGLProgram()->setUniformVector3F("inputViewDirection", calculateForwardVector(getRotation()));
    GLProgram::getGLProgram()->setUniformVector3F("inputViewPosition", getLocation());
}


void Camera::bindControl(KeyHandler *keyHandler) {

    float speed = 5;

    keyHandler->bindOnMouseMotionEvent([this](glm::vec2 position, glm::vec2 offset) -> void {
        this->rotation.x += offset.y;

        if (this->rotation.x < -90) {
            this->rotation.x = -90;
        } else if (this->rotation.x > 90) {
            this->rotation.x = 90;
        }

        this->rotation.y += offset.x;

        if (this->rotation.y < -180) {
            this->rotation.y = 360 - this->rotation.y;
        } else if (this->rotation.x > 180) {
            this->rotation.y = -360 + this->rotation.y;
        }
    });

    keyHandler->bindOnPressedEvent([this, speed]() -> void {
        glm::vec3 forwardVector = calculateForwardVector(this->rotation);
        this->location += forwardVector * speed;
    }, SDLK_w);

    keyHandler->bindOnPressedEvent([this, speed]() -> void {
        glm::vec3 forwardVector = calculateForwardVector(this->rotation);
        this->location -= forwardVector * speed;
    }, SDLK_s);

    keyHandler->bindOnPressedEvent([this, speed]() -> void {
        glm::vec3 rightVector = calculateRightVector(this->rotation);
        this->location -= rightVector * speed;
    }, SDLK_d);

    keyHandler->bindOnPressedEvent([this, speed]() -> void {
        glm::vec3 rightVector = calculateRightVector(this->rotation);
        this->location += rightVector * speed;
    }, SDLK_a);

    keyHandler->bindOnPressedEvent([this, speed]() -> void {
        glm::vec3 upVector = calculateUpVector(this->rotation);
        this->location -= upVector * speed;
    }, SDLK_SPACE);

    keyHandler->bindOnPressEvent([this]() -> void {
        std::cout << "Location: " << this->location.x << " " << this->location.y << " " << this->location.z
                  << std::endl;
        std::cout << "Rotation: " << this->rotation.x << " " << this->rotation.y << " " << this->rotation.z
                  << std::endl;
    }, SDLK_f);
}

float Camera::getFov() {
    if (type == SHADOW || DEBUG) {
        return LightRender::getLightRenderer()->getLightFov();
    }

    return fov;
}

float Camera::getFar() {
    return far;
}

float Camera::getNear() {
    return near;
}
