//
// Created by Bogdan Madzhuga on 14.10.2023.
//

#ifndef PRACTICE5_TEXTURE_H
#define PRACTICE5_TEXTURE_H

#include <GL/glew.h>
#include "filesystem"
#include "structures/Vector3F.h"

class Texture {

private:
    std::string TAG = "Texture";
    std::string layer = "textureLayer";
    unsigned char *imageData;
    GLuint textureID;

    Vector3F albedo = Vector3F(0.7f, 0.5f, 0.3f);
    float roughness = 0.3f;
    float glossiness = 5.f;

    bool bindable = false;

public:
    int width = 1024, height = 1024, numChannels = 3;


public:
    Texture(std::filesystem::path const &path);

    Texture();

    unsigned char *getImageData();

    GLuint getTextureID();

    std::string getLayerName();

    Vector3F getAlbedo();

    float getGlossiness();

    float getRoughness();

    bool isBindable() {
        return bindable;
    }

    float setRoughness(float value);

    float setGlossiness(float value);
};


#endif //PRACTICE5_TEXTURE_H
