//
// Created by Bogdan Madzhuga on 14.10.2023.
//

#ifndef PRACTICE5_TEXTURE_H
#define PRACTICE5_TEXTURE_H

#include <GL/glew.h>
#include "filesystem"

class Texture {

private:
    std::string TAG = "Texture";
    std::string layer = "textureLayer";
    unsigned char *imageData;
    GLuint textureID;

public:
    int width = 1024, height = 1024, numChannels = 3;


public:
    Texture(std::filesystem::path const &path);

    unsigned char *getImageData();

    GLuint getTextureID();

    std::string getLayerName();
};


#endif //PRACTICE5_TEXTURE_H
