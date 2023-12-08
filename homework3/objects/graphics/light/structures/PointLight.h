//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_POINTLIGHT_H
#define HOMEWORK2_POINTLIGHT_H

#include "glm/vec3.hpp"

struct PointLight {
    glm::vec3 location = {0, 0, 0};
    glm::vec3 color = {1, 1, 1};
    glm::vec3 attenuation = {1, 1, 1};

    float range = 0;
};

#endif //HOMEWORK2_POINTLIGHT_H
