//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_DIRECTIONLIGHT_H
#define HOMEWORK2_DIRECTIONLIGHT_H

#include "glm/vec3.hpp"

struct DirectionLight {
    glm::vec3 location = {0, 0, 0};
    glm::vec3 direction = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 color = {1, 1, 1};
    glm::vec3 attenuation = {1, 1, 1};

    float range = 2000;
    float fov = 45;
};

#endif //HOMEWORK2_DIRECTIONLIGHT_H
