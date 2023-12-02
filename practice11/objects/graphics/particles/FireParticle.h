//
// Created by Bogdan Madzhuga on 02.12.2023.
//

#ifndef PRACTICE11_FIREPARTICLE_H
#define PRACTICE11_FIREPARTICLE_H


#include "objects/graphics/renderable/Placeable.h"
#include <glm/gtx/quaternion.hpp>
#include <random>
#include <cmath>

class FireParticle {

private:
    int details = 256;
    float A = 1, C = 2, D = 0.5;
    Placeable *placeable;
    std::vector<Particle> particles;
    std::default_random_engine rng;

public:
    FireParticle();

    FireParticle(int details);

private:
    void moveParticles(float dt);

    void generateParticles();

public:
    void moveAndDraw(float dt);

    virtual void setPosition(glm::vec3 position);

    virtual void addRotation(glm::vec3 offset);

    virtual void setRotation(glm::vec3 rotation);

    virtual void setScale(glm::vec3 scale);

    virtual Material *getMaterial() final;

    virtual Placeable* getPlaceable() final;
};


#endif //PRACTICE11_FIREPARTICLE_H
