//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SUN_H
#define HOMEWORK2_SUN_H


#include "objects/graphics/light/structures/DirectionLight.h"
#include "utils/MathUtils.h"
#include "glm/geometric.hpp"

class Sun {
private:
    static Sun *instance;
    DirectionLight light = DirectionLight{};
    float height = 100;

public:
    Sun();

public:
    // Methods

    static Sun *getSun() {
        return instance;
    }

    void setColor(glm::vec3 color) {
        light.color = color;
    }

    void setDirection(glm::vec3 direction) {
        light.direction = glm::normalize(direction);
    }

    glm::vec3 getRotation() {
        glm::vec3 rotation = directionToRotation(light.direction);
        return rotation;
    }

    glm::vec3 getLocation() {
        return height * light.direction;
    }

    float getHeight(){
        return height;
    }

    glm::vec3 getColor() {
        return light.color;
    }

    glm::vec3 getDirection() {
        return light.direction;
    }

    void bindLight();
};


#endif //HOMEWORK2_SUN_H
