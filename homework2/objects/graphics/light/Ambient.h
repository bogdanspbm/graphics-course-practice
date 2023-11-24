//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_AMBIENT_H
#define HOMEWORK2_AMBIENT_H


#include "objects/graphics/light/structures/PointLight.h"

class Ambient {
private:
    static Ambient *instance;
    PointLight light = PointLight{};

public:
    Ambient();

public:
    // Methods
    void setColor(glm::vec3 color) {
        light.color = color;
    }

    void bindLight();

    static Ambient *getAmbient() {
        return instance;
    }
};


#endif //HOMEWORK2_AMBIENT_H
