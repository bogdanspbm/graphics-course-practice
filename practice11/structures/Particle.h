//
// Created by Bogdan Madzhuga on 02.12.2023.
//

#ifndef PRACTICE11_PARTICLE_H
#define PRACTICE11_PARTICLE_H

#include "glm/vec3.hpp"
#include <random>

struct Particle {
    glm::vec3 position = {0, 0, 0};
    float rotation = 0;
    float size = 1;
    glm::vec3 color = {1, 1, 1};

    glm::vec3 velocity = {0, 1, 0};
    float angularVelocity = 1;

    Particle(std::default_random_engine &rng) {
        position.x = std::uniform_real_distribution<float>{-1.f, 1.f}(rng);
        position.y = 0.f;
        position.z = std::uniform_real_distribution<float>{-1.f, 1.f}(rng);
        rotation = std::uniform_real_distribution<float>{-90.f, 90.f}(rng);
        size = std::uniform_real_distribution<float>{0.003f, 0.05f}(rng);
        color.x = std::uniform_real_distribution<float>{0.f, 1.f}(rng);
        color.y = std::uniform_real_distribution<float>{0.f, 1.f}(rng);
        color.z = std::uniform_real_distribution<float>{0.f, 1.f}(rng);
    }
};

#endif //PRACTICE11_PARTICLE_H
