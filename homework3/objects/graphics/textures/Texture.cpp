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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width / 2, height / 2, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Texture::Texture(std::filesystem::path const &path, TextureType textureType) {
    this->textureType = textureType;
    if (this->is2D()) {
        this->loadTexture2D(path, textureType);
    } else {
        this->loadTexture3D(path, textureType);
    }
}

Texture::Texture(const std::filesystem::path &path) {
    if (this->is2D()) {
        this->loadTexture2D(path, this->textureType);
    } else {
        this->loadTexture3D(path, this->textureType);
    }
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
    if (is2D()) {
        glBindTexture(GL_TEXTURE_2D, getTextureID());
    } else {
        glBindTexture(GL_TEXTURE_3D, getTextureID());
    }

    // Get the uniform location for the specific texture unit
    std::string uniformName = "texture" + std::to_string(textureUnit - GL_TEXTURE0);
    int textureLocation = glGetUniformLocation(GLProgram::getGLProgram()->getProgramID(), uniformName.c_str());

    if (textureLocation != -1) {
        glUniform1i(textureLocation,
                    textureUnit - GL_TEXTURE0); // Set the uniform to the texture unit offset from GL_TEXTURE0
        GLProgram::getGLProgram()->setUniformInt(
                ("enabledTextures[" + std::to_string(textureUnit - GL_TEXTURE0) + "]").c_str(), 1);
        GLProgram::getGLProgram()->setUniformInt(
                ("textureChannels[" + std::to_string(textureUnit - GL_TEXTURE0) + "]").c_str(), this->numChannels);
    }

}

void Texture::setTextureType(TextureType type) {
    this->textureType = type;
}

void Texture::loadTexture2D(const std::filesystem::path &path, TextureType type) {
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
    } else if (numChannels == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE,
                     getImageData());
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);

    cachedTextures[path] = this;
}

void Texture::loadTexture3D(const std::filesystem::path &path, TextureType type) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    this->width = 128;
    this->height = 64;
    this->numChannels = 64;

    std::vector<char> pixels(this->width * this->height * this->numChannels);
    std::ifstream input(path, std::ios::binary);
    input.read(pixels.data(), pixels.size());

    glTexImage3D(GL_TEXTURE_3D,
                 0,
                 GL_R8,
                 this->width, this->height, this->numChannels,
                 0,
                 GL_RED, GL_UNSIGNED_BYTE, pixels.data());


}

bool Texture::is2D() {
    return textureType < DEFAULT_3D || textureType == SHADOW_MAP;
}
