//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Sun.h"
#include "objects/opengl/GLProgram.h"

Sun *Sun::instance = new Sun();

Sun::Sun() {

}

void Sun::bindLight() {
    GLProgram::getGLProgram()->setUniformVector3F("inputSunColor", glm::normalize(light.color));
    GLProgram::getGLProgram()->setUniformVector3F("lightDirection", getDirection());

    if (light.useRotation) {
        GLProgram::getGLProgram()->setUniformVector3F("inputSunDirection", -glm::normalize(calculateForwardVector(light.rotation)));
    } else {
        GLProgram::getGLProgram()->setUniformVector3F("inputSunDirection", -glm::normalize(light.direction));
    }
}
