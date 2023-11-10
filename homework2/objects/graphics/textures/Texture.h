//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_TEXTURE_H
#define HOMEWORK2_TEXTURE_H

#include <map>
#include <filesystem>
#include "libs/stb_image.h"
#include "enums/GLTextureType.h"
#include <GL/glew.h>

class Texture {
private:
    static std::map<std::string, Texture *> cachedTextures;

    std::string TAG = "Texture";
    std::string name = "";

    int width = 1024, height = 1024, numChannels = 3;

    unsigned char *imageData;
    GLuint textureID;
    TextureType textureType;

    Texture(std::filesystem::path const &path);
    Texture(std::filesystem::path const &path, TextureType textureType);

public:
    Texture();

    Texture(int width, int height);

    static Texture *getTexture(std::filesystem::path const &path, TextureType type) {
        if (cachedTextures.contains(path)) {
            return cachedTextures[path];
        }

        Texture *texture = new Texture(path, type);
        cachedTextures[path] = texture;
        return texture;
    }

public:
    // Getters
    unsigned char *getImageData();

    GLuint getTextureID();

    void bindTexture();

    void setTextureType(TextureType type);

    TextureType getType(){
        return this->textureType;
    }

};


#endif //HOMEWORK2_TEXTURE_H
