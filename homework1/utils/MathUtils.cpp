//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include <array>
#include "MathUtils.h"

std::array<float, 3> positionToNormal(std::array<float, 3> position) {
    float length = std::sqrt(position[0] * position[0] + position[1] * position[1] + position[2] * position[2]);
    if (length < 1e-6) {
        return {0.0f, 0.0f, 1.0f};
    }

    std::array<float, 3> normal = {
            -position[1] / length,
            position[0] / length,
            0.0f
    };

    return normal;
}

Vector3D calculateForwardVector(Vector3D rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(Vector3D(0, 0, 1), yaw, pitch);
}

Vector3D calculateRightVector(const Vector3D rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(Vector3D(1, 0, 0), yaw, pitch);
}

Vector3D rotateVector(const Vector3D vector, float yaw, float pitch) {
    // Convert angles from degrees to radians
    float yawRad = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;

    // Calculate the rotation matrix for yaw and pitch
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);
    float cosPitch = cosf(pitchRad);
    float sinPitch = sinf(pitchRad);

    // Apply the rotation to the vector
    Vector3D rotatedVector;
    rotatedVector.x = vector.x * (cosYaw * cosPitch) + vector.z * (sinYaw * cosPitch);
    rotatedVector.y = vector.y * cosPitch - vector.z * sinPitch;
    rotatedVector.z = -vector.x * (sinYaw * cosPitch) + vector.z * (cosYaw * cosPitch);

    return rotatedVector;
}