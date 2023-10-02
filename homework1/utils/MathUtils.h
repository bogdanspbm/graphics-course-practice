//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef HOMEWORK1_MATHUTILS_H
#define HOMEWORK1_MATHUTILS_H

#include <vector>
#include "structures/Vector3D.h"

std::array<float, 3> positionToNormal(std::array<float, 3> position);

Vector3D calculateForwardVector(Vector3D rotation);

Vector3D calculateRightVector(const Vector3D rotation);

Vector3D rotateVector(const Vector3D vector, float yaw, float pitch);

#endif //HOMEWORK1_MATHUTILS_H
