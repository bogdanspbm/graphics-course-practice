//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_GLTEXTURETYPE_H
#define HOMEWORK2_GLTEXTURETYPE_H

#include <GL/glew.h>

enum TextureType{
    DEFAULT,
    NORMAL_MAP,
    GLOSS_MAP,
    ROUGH_MAP,
    SHADOW_MAP
};

static int getTextureUnit(TextureType type){
    switch (type) {
        case DEFAULT:
            return 0 + GL_TEXTURE0;
        case NORMAL_MAP:
            return 1 + GL_TEXTURE0;
        case GLOSS_MAP:
            return 2 + GL_TEXTURE0;
        case ROUGH_MAP:
            return 3 + GL_TEXTURE0;
        case SHADOW_MAP:
            return 31 + GL_TEXTURE0;
    }

    return 0 + GL_TEXTURE0;
}

#endif //HOMEWORK2_GLTEXTURETYPE_H
