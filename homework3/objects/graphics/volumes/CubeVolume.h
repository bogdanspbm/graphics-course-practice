//
// Created by Bogdan Madzhuga on 08.12.2023.
//

#ifndef PRACTICE12_CUBEVOLUME_H
#define PRACTICE12_CUBEVOLUME_H


#include "objects/graphics/renderable/Placeable.h"

class CubeVolume {
private:
    glm::vec3 boxMin{-2.f, -1.f, -1.f};
    glm::vec3 boxMax{2.f, 1.f, 1.f};
    glm::vec3 absorption = {1, 1, 1};
    glm::vec3 scattering = {4, 1, 10};

    Placeable *placeable;
public:
    CubeVolume();

    Placeable *getPlaceable();

    void draw();

};


#endif //PRACTICE12_CUBEVOLUME_H
