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

Camera::Camera(ProgramType type) {
    this->type = type;
}

const glm::vec3 &Camera::getLocation() const {
    if (type == SHADOW) {
        return Sun::getSun()->getLocation();
    }
    return location;
}

void Camera::setLocation(const glm::vec3 &location) {
    Camera::location = location;
}

const glm::vec3 &Camera::getRotation() const {
    if (type == SHADOW) {
        return Sun::getSun()->getRotation();
    }
    return rotation;
}

void Camera::setRotation(const glm::vec3 &rotation) {
    Camera::rotation = rotation;
}


void Camera::calcViewMatrix(float *matrix) {
    memset(matrix, 0, sizeof(float) * 16);

    glm::vec3 rotation = getRotation();

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                               * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),
                                                 glm::vec3(-getLocation().x, -getLocation().y, -getLocation().z));

    glm::mat4 viewMatrix = rotationMatrix * translationMatrix;

    glm::mat4 transposedViewMatrix = glm::transpose(viewMatrix);

    memcpy(matrix, &transposedViewMatrix[0][0], sizeof(float) * 16);
}

void Camera::calcProjectionMatrix(float matrix[16]) {
    float right = near * tan(fov / 2.0f);
    float aspectRatio = (float) width / (float) height;
    float top = right / aspectRatio;

    memset(matrix, 0, sizeof(matrix));
    matrix[0] = (2.0f * near) / (right * 2.0f);
    matrix[1] = 0.f;
    matrix[2] = 0.f;
    matrix[3] = 0.f;

    matrix[4] = 0.f;
    matrix[5] = (2.0f * near) / (top * 2.0f);
    matrix[6] = 0.f;
    matrix[7] = 0.f;

    matrix[8] = 0.f;
    matrix[9] = 0.f;
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -(2.0f * far * near) / (far - near);

    matrix[12] = 0.f;
    matrix[13] = 0.f;
    matrix[14] = -1.f;
    matrix[15] = 0.f;
}

void Camera::bindView() {
    GLProgram::getGLProgram()->setUniformFloat("far", far);
    GLProgram::getGLProgram()->setUniformFloat("near", near);
    GLProgram::getGLProgram()->setUniformVector3F("inputViewDirection", calculateForwardVector(getRotation()));
    GLProgram::getGLProgram()->setUniformVector3F("inputViewPosition", getLocation());
}

void Camera::bindControl(KeyHandler *keyHandler) {

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

    keyHandler->bindOnPressedEvent([this]() -> void {
        glm::vec3 forwardVector = calculateForwardVector(this->rotation);
        this->location -= forwardVector;
    }, SDLK_w);

    keyHandler->bindOnPressedEvent([this]() -> void {
        glm::vec3 forwardVector = calculateForwardVector(this->rotation);
        this->location += forwardVector;
    }, SDLK_s);

    keyHandler->bindOnPressedEvent([this]() -> void {
        glm::vec3 rightVector = calculateRightVector(this->rotation);
        this->location += rightVector;
    }, SDLK_d);

    keyHandler->bindOnPressedEvent([this]() -> void {
        glm::vec3 rightVector = calculateRightVector(this->rotation);
        this->location -= rightVector;
    }, SDLK_a);
}