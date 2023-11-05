//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef HOMEWORK1_MATHUTILS_H
#define HOMEWORK1_MATHUTILS_H

#include <vector>
#include "structures/Vector3F.h"

Vector3F positionToNormal(Vector3F position);

Vector3F calculateForwardVector(Vector3F rotation);

Vector3F calculateRightVector(const Vector3F rotation);

Vector3F rotateVector(const Vector3F vector, float yaw, float pitch);

Vector3F linearInterpolation(Vector3F a, Vector3F b, float percentage);

#endif //HOMEWORK1_MATHUTILS_H
