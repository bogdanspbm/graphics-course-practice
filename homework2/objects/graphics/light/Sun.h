//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SUN_H
#define HOMEWORK2_SUN_H


#include "objects/graphics/light/structures/DirectionLight.h"

class Sun {
private:
    static Sun *instance;
    DirectionLight light = DirectionLight{};

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
        light.direction = direction;
    }

    glm::vec3 getRotation() {
        return light.direction;
    }

    void bindLight();
};


#endif //HOMEWORK2_SUN_H
