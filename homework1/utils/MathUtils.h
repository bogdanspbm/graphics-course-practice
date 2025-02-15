//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef HOMEWORK1_MATHUTILS_H
#define HOMEWORK1_MATHUTILS_H

#include <vector>
#include "structures/Vector3D.h"

Vector3D positionToNormal(Vector3D position);

Vector3D calculateForwardVector(Vector3D rotation);

Vector3D calculateRightVector(const Vector3D rotation);

Vector3D rotateVector(const Vector3D vector, float yaw, float pitch);

Vector3D linearInterpolation(Vector3D a, Vector3D b, float percentage);

#endif //HOMEWORK1_MATHUTILS_H
