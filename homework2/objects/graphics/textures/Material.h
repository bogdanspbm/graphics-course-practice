//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_MATERIAL_H
#define HOMEWORK2_MATERIAL_H

#include <vector>
#include <map>
#include "objects/graphics/textures/Texture.h"
#include "glm/vec3.hpp"


class Material {
private:
    static std::map<std::string, Material *> cachedMaterials;
    std::vector<Texture *> textures;
    std::string name;
    glm::vec3 albedo = {0.6, 0.6, 0.6};
    float roughness = 0.1f;
    float glossiness = 0.5f;
    float opacity = 1.f;

public:
    static Material *getMaterialByName(std::string name) {
        if (cachedMaterials.contains(name)) {
            return cachedMaterials[name];
        }

        Material *material = new Material();
        material->name = name;
        cachedMaterials[name] = material;
        return material;
    }

public:
    // Methods
    void bindMaterial();

    void clearTextures();

    void addTexture(Texture *texture);

    void setTexture(int index, Texture *texture);

    void setAlbedo(const glm::vec3 &albedo);

    void setRoughness(float roughness);

    void setOpacity(float opacity);

    void setGlossiness(float glossiness);
};


#endif //HOMEWORK2_MATERIAL_H
