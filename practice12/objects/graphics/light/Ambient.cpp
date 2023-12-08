//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Ambient.h"
#include "objects/opengl/GLProgram.h"

Ambient* Ambient::instance = new Ambient();

void Ambient::bindLight() {
    GLProgram::getGLProgram()->setUniformVector3F("inputAmbientLight", light.color);
}

Ambient::Ambient() {

}
