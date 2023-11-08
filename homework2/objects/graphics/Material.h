//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_MATERIAL_H
#define HOMEWORK2_MATERIAL_H

#include <vector>
#include "Texture.h"
#include "glm/vec3.hpp"


class Material {
private:
    std::vector<Texture *> textures;
    glm::vec3 albedo = {1, 1, 1};
    float roughness = 1.f;
    float glossiness = 1.f;

public:
    // Methods
    void bindMaterial();

    void addTexture(Texture *texture);

    void setTexture(int index, Texture *texture);
};


#endif //HOMEWORK2_MATERIAL_H
