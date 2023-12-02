//
// Created by Bogdan Madzhuga on 02.12.2023.
//

#ifndef PRACTICE11_PARTICLE_H
#define PRACTICE11_PARTICLE_H

#include "glm/vec3.hpp"

struct Particle
{
    glm::vec3 position;
    float rotation;
    float size;

    glm::vec3 speed;
    glm::vec3 velocity;
    float rotationSpeed;
    float angularVelocity;
};

#endif //PRACTICE11_PARTICLE_H
