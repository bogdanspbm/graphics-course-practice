#include "Texture.h"
#include "libs/stb_image.h"

Texture::Texture() {
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width / 2, height / 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::filesystem::path &path) {
    bindable = true;
    imageData = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

    if (imageData == nullptr) {
        throw std::invalid_argument(TAG + ": Can't read texture.");
    }

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (numChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     getImageData());
    } else if (numChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     getImageData());
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imageData);
}

GLuint Texture::getTextureID() {
    return textureID;
}

std::string Texture::getLayerName() {
    return layer;
}

unsigned char *Texture::getImageData() {
    return imageData;
}

Vector3F Texture::getAlbedo() {
    return this->albedo;
}

float Texture::getGlossiness() {
    return glossiness;
}

float Texture::getRoughness() {
    return roughness;
}


float Texture::setRoughness(float value) {
    this->roughness = value;
}

float Texture::setGlossiness(float value) {
    this->glossiness = value;
}