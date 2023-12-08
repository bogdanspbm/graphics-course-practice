#pragma once


#ifndef HOMEWORK2_MATHUTILS_H
#define HOMEWORK2_MATHUTILS_H

#include <valarray>
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"


inline glm::vec3 positionToNormal(glm::vec3 position) {
    float length = std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    if (length < 1e-6) {
        return {0.0f, 0.0f, 1.0f};
    }

    glm::vec3 normal = {
            -position.y / length,
            position.x / length,
            0.0f
    };

    return normal;
}

inline glm::vec3 calculateForwardVector(glm::vec3 rotation) {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), {1.f, 0.f, 0.f});
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), {0.f, 1.f, 0.f});

    glm::vec3 forwardVector = glm::vec3(rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 0.f));
    return glm::normalize(forwardVector);
}

inline glm::vec3 calculateRightVector(const glm::vec3 rotation) {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), {1.f, 0.f, 0.f});
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), {0.f, 1.f, 0.f});

    glm::vec3 rightVector = glm::vec3(rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 0.f));
    return glm::normalize(rightVector);
}

inline glm::vec3 calculateUpVector(const glm::vec3 rotation) {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), {1.f, 0.f, 0.f});
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), {0.f, 1.f, 0.f});

    glm::vec3 upVector = glm::vec3(rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 0.f));
    return glm::normalize(upVector);
}


inline glm::vec3 linearInterpolation(glm::vec3 a, glm::vec3 b, float percentage) {
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    float interpolatedX = a.x + percentage * (b.x - a.x);
    float interpolatedY = a.y + percentage * (b.y - a.y);
    float interpolatedZ = a.z + percentage * (b.z - a.z);

    return glm::vec3(interpolatedX, interpolatedY, interpolatedZ);
}

inline glm::vec3 directionToRotation(const glm::vec3 &direction) {
    glm::vec3 rotation;

    // Calculate yaw (rotation around the vertical Y-axis)
    rotation.y = atan2(direction.x, -direction.z) * (180.0f / M_PI);

    // Calculate pitch (rotation around the X-axis)
    rotation.x = atan2(direction.y, -direction.z) * (180.0f / M_PI);


    return rotation;
}

inline float clampTo180(float value) {
    while (value > 180.0f) {
        value -= 360.0f;
    }
    while (value < -180.0f) {
        value += 360.0f;
    }
    return value;
}

inline glm::vec3 clampedVec(glm::vec3 originalVec) {
    return {clampTo180(originalVec.x),
            clampTo180(originalVec.y),
            clampTo180(originalVec.z)};
};

inline glm::vec3 cross(glm::vec3 a, glm::vec3 b)  {
    return glm::vec3{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    };
}

#endif //HOMEWORK2_MATHUTILS_H
