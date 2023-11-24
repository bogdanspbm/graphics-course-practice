//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SUN_H
#define HOMEWORK2_SUN_H


#include "objects/graphics/light/structures/DirectionLight.h"
#include "utils/MathUtils.h"
#include <iostream>
#include "glm/geometric.hpp"

class Sun {
private:
    static Sun *instance;
    DirectionLight light = DirectionLight{};
    float height = 4;

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
        if(light.useRotation){
            return light.rotation;
        }

        glm::vec3 rotation = directionToRotation(light.direction);
        return rotation;
    }

    void setRotation(glm::vec3 rotation) {
        light.rotation = clampedVec(rotation);
        std::cout << light.rotation.y << std::endl;
        light.useRotation = true;
    }

    glm::vec3 getLocation() {
        return height * getDirection();
    }

    float getHeight(){
        return height;
    }

    glm::vec3 getColor() {
        return light.color;
    }

    glm::vec3 getDirection() {
        if(!light.useRotation){
            return light.direction;
        }

        glm::vec3 direction = glm::normalize(calculateForwardVector(light.rotation));
        return direction;
    }

    void bindLight();
};


#endif //HOMEWORK2_SUN_H
