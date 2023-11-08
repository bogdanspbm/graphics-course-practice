//
// Created by Bogdan Madzhuga on 28.10.2023.
//

#ifndef PRACTICE7_LIGHTPOINT_H
#define PRACTICE7_LIGHTPOINT_H

#include "Vector3F.h"

#endif //PRACTICE7_LIGHTPOINT_H

struct PointLight {
    Vector3F position = Vector3F();
    Vector3F color = Vector3F();
    Vector3F attenuation = Vector3F();
};