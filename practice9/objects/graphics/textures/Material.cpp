//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Material.h"
#include "objects/opengl/GLProgram.h"

std::map<std::string, Material *> Material::cachedMaterials;

void Material::bindMaterial() {
    clearTextures();

    bool useAlbedo = true;

    for (int i = 0; i < textures.size(); i++) {
        auto texture = textures[i];
        texture->bindTexture();

        if (texture->getType() == DEFAULT) {
            useAlbedo = false;
        }
    }


    if(useAlbedo){
        GLProgram::getGLProgram()->setUniformInt("useAlbedo", 1);
    } else {
        GLProgram::getGLProgram()->setUniformInt("useAlbedo", 0);
    }

    GLProgram::getGLProgram(SHADOW)->getFrameBuffer()->getTexture()->bindTexture();

    GLProgram::getGLProgram()->setUniformVector3F("inputAlbedo", albedo);
    GLProgram::getGLProgram()->setUniformFloat("roughness", roughness);
    GLProgram::getGLProgram()->setUniformFloat("opacity", opacity);
    GLProgram::getGLProgram()->setUniformFloat("glossiness", glossiness);
}

void Material::addTexture(Texture *texture) {
    if (hasTextureWithType(texture->getType())) {
        return;
    }
    textures.push_back(texture);
}

bool Material::hasTextureWithType(TextureType type) {
    for (auto texture: textures) {
        if (texture->getType() == type) {
            return true;
        }
    }
    return false;
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

void Material::clearTextures() {
    for (int i = 0; i < TextureType::SHADOW_MAP; i++) {
        TextureType type = static_cast<TextureType>(i);
        Texture *texture = new Texture();

        if (type == DISPLACEMENT_MAP) {
            GLProgram::getGLProgram()->setUniformFloat("useDisplacementMap", 0.3f);
        }

        texture->setTextureType(type);
        texture->bindTexture();
    }
}
