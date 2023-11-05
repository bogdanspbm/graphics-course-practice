//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include <array>
#include "MathUtils.h"

Vector3F positionToNormal(Vector3F position) {
    float length = std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    if (length < 1e-6) {
        return {0.0f, 0.0f, 1.0f};
    }

    Vector3F normal = {
            -position.y / length,
            position.x / length,
            0.0f
    };

    return normal;
}

Vector3F calculateForwardVector(Vector3F rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(Vector3F(0, 0, 1), yaw, pitch);
}

Vector3F calculateRightVector(const Vector3F rotation) {
    float yaw = rotation.y * M_PI / 180.0f;
    float pitch = rotation.x * M_PI / 180.0f;

    return rotateVector(Vector3F(1, 0, 0), yaw, pitch);
}

Vector3F rotateVector(const Vector3F vector, float yaw, float pitch) {
    // Convert angles from degrees to radians
    float yawRad = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;

    // Calculate the rotation matrix for yaw and pitch
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);
    float cosPitch = cosf(pitchRad);
    float sinPitch = sinf(pitchRad);

    // Apply the rotation to the vector
    Vector3F rotatedVector;
    rotatedVector.x = vector.x * (cosYaw * cosPitch) + vector.z * (sinYaw * cosPitch);
    rotatedVector.y = vector.y * cosPitch - vector.z * sinPitch;
    rotatedVector.z = -vector.x * (sinYaw * cosPitch) + vector.z * (cosYaw * cosPitch);

    return rotatedVector;
}

Vector3F linearInterpolation(Vector3F a, Vector3F b, float percentage) {
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    float interpolatedX = a.x + percentage * (b.x - a.x);
    float interpolatedY = a.y + percentage * (b.y - a.y);
    float interpolatedZ = a.z + percentage * (b.z - a.z);

    return Vector3F(interpolatedX, interpolatedY, interpolatedZ);
}