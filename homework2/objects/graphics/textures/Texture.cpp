//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Texture.h"
#include "objects/opengl/GLProgram.h"

std::map<std::string, Texture *> Texture::cachedTextures;

Texture::Texture() {
    glGenTextures(1, &textureID);

    this->textureType = SHADOW_MAP;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height) {

    this->width = width;
    this->height = height;

    this->textureType = SHADOW_MAP;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::filesystem::path const &path, TextureType textureType) {
    this->name = path;
    this->textureType = textureType;

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

    cachedTextures[path] = this;
}

Texture::Texture(const std::filesystem::path &path) {
    this->name = path;
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

    cachedTextures[path] = this;
}

unsigned char *Texture::getImageData() {
    return imageData;
}

GLuint Texture::getTextureID() {
    return textureID;
}

void Texture::bindTexture() {
    int textureUnit = getTextureUnit(this->textureType);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, getTextureID());

    // Get the uniform location for the specific texture unit
    std::string uniformName = "texture" + std::to_string(textureUnit - GL_TEXTURE0);
    int textureLocation = glGetUniformLocation(GLProgram::getGLProgram()->getProgramID(), uniformName.c_str());

    if (textureLocation != -1) {
        glUniform1i(textureLocation, textureUnit - GL_TEXTURE0); // Set the uniform to the texture unit offset from GL_TEXTURE0

        if (textureType == DISPLACEMENT_MAP && name != "") {
            GLProgram::getGLProgram()->setUniformFloat("useDisplacementMap", 1);
        }
    }

}

void Texture::setTextureType(TextureType type) {
    this->textureType = type;
}
