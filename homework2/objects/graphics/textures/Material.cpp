//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Material.h"
#include "objects/opengl/GLProgram.h"

std::map<std::string, Material *> Material::cachedMaterials;

void Material::bindMaterial() {

    if (textures.empty()) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (int i = 0; i < textures.size(); i++) {
        auto texture = textures[i];
        texture->bindTexture();
    }

    GLProgram::getGLProgram()->setUniformInt("texturesCount", textures.size());
    GLProgram::getGLProgram()->setUniformVector3F("inputAlbedo", albedo);
    GLProgram::getGLProgram()->setUniformFloat("roughness", roughness);
    GLProgram::getGLProgram()->setUniformFloat("glossiness", glossiness);
}

void Material::addTexture(Texture *texture) {
    textures.push_back(texture);
}

void Material::setTexture(int index, Texture *texture) {
    textures[index] = texture;
}

void Material::setAlbedo(const glm::vec3 &albedo) {
    Material::albedo = albedo;
}

void Material::setRoughness(float roughness) {
    Material::roughness = roughness;
}

void Material::setGlossiness(float glossiness) {
    Material::glossiness = glossiness;
}

void Material::setOpacity(float opacity) {
    Material::opacity = opacity;
}
