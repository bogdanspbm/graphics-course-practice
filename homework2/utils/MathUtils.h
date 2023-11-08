//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_MATHUTILS_H
#define HOMEWORK2_MATHUTILS_H

#include <valarray>
#include "glm/vec3.hpp"

glm::vec3 rotateVector(const glm::vec3 vector, float yaw, float pitch) {
    // Convert angles from degrees to radians
    float yawRad = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;

    // Calculate the rotation matrix for yaw and pitch
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);
    float cosPitch = cosf(pitchRad);
    float sinPitch = sinf(pitchRad);

    // Apply the rotation to the vector
    glm::vec3 rotatedVector;
    rotatedVector.x = vector.x * (cosYaw * cosPitch) + vector.z * (sinYaw * cosPitch);
    rotatedVector.y = vector.y * cosPitch - vector.z * sinPitch;
    rotatedVector.z = -vector.x * (sinYaw * cosPitch) + vector.z * (cosYaw * cosPitch);

    return rotatedVector;
}

glm::vec3 positionToNormal(glm::vec3 position) {
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

glm::vec3 calculateForwardVector(glm::vec3 rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(glm::vec3(0, 0, 1), yaw, pitch);
}

glm::vec3 calculateRightVector(const glm::vec3 rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(glm::vec3(1, 0, 0), yaw, pitch);
}


glm::vec3 linearInterpolation(glm::vec3 a, glm::vec3 b, float percentage) {
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    float interpolatedX = a.x + percentage * (b.x - a.x);
    float interpolatedY = a.y + percentage * (b.y - a.y);
    float interpolatedZ = a.z + percentage * (b.z - a.z);

    return glm::vec3(interpolatedX, interpolatedY, interpolatedZ);
}

#endif //HOMEWORK2_MATHUTILS_H
