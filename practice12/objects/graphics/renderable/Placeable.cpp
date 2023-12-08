//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Placeable.h"

void Placeable::draw() {
    float modelMatrix[16];
    this->calcModelMatrix(modelMatrix);
    GLProgram::getGLProgram()->setUniformMatrix4FV("model", modelMatrix, true);
    this->renderable->draw();
}

void Placeable::setPosition(glm::vec3 position) {
    this->position = position;
}

void Placeable::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
}

void Placeable::setScale(glm::vec3 scale) {
    this->scale = scale;
}

void Placeable::calcModelMatrix(float modelMatrix[16]){
    memset(modelMatrix, 0, sizeof(modelMatrix));
    modelMatrix[0] = 1.0f;
    modelMatrix[5] = 1.0f;
    modelMatrix[10] = 1.0f;
    modelMatrix[15] = 1.0f;

    // Apply translation
    modelMatrix[3] = position.x;
    modelMatrix[7] = position.y;
    modelMatrix[11] = position.z;

    // Apply rotation (assuming rotation is in degrees)
    float radX = rotation.x * M_PI / 180.0f;
    float radY = rotation.y * M_PI / 180.0f;
    float radZ = rotation.z * M_PI / 180.0f;

    float cosX = cosf(radX);
    float sinX = sinf(radX);
    float cosY = cosf(radY);
    float sinY = sinf(radY);
    float cosZ = cosf(radZ);
    float sinZ = sinf(radZ);

    modelMatrix[0] = cosY * cosZ * scale.x;
    modelMatrix[1] = (cosX * sinZ + sinX * sinY * cosZ) * scale.y;
    modelMatrix[2] = (sinX * sinZ - cosX * sinY * cosZ) * scale.z;

    modelMatrix[4] = -cosY * sinZ * scale.x;
    modelMatrix[5] = (cosX * cosZ - sinX * sinY * sinZ) * scale.y;
    modelMatrix[6] = (sinX * cosZ + cosX * sinY * sinZ) * scale.z;

    modelMatrix[8] = sinY * scale.x;
    modelMatrix[9] = (-sinX * cosY) * scale.y;
    modelMatrix[10] = (cosX * cosY) * scale.z;
}

Placeable::Placeable(const std::filesystem::path &path) {
    this->renderable = Renderable::getRenderableByPath(path);
}

Material *Placeable::getMaterial() {
    return this->renderable->getMaterial();
}

Placeable::Placeable(Renderable *renderable) {
    this->renderable = renderable;
}

void Placeable::addRotation(glm::vec3 offset) {
        this->rotation += offset;
}

Renderable *Placeable::GetRenderable() {
    return renderable;
}
