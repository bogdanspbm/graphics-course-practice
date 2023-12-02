//
// Created by Bogdan Madzhuga on 02.12.2023.
//

#ifndef PRACTICE11_PARTICLE_H
#define PRACTICE11_PARTICLE_H

#include "glm/vec3.hpp"

struct Particle
{
    glm::vec3 position = {0,0,0};
    float rotation = 0;
    float size = 1;
    glm::vec3 color = {1,1,1};

    glm::vec3 speed = {0,1,0};
    glm::vec3 velocity = {0,1,0};
    float rotationSpeed = 1;
    float angularVelocity = 1;
};

#endif //PRACTICE11_PARTICLE_H
