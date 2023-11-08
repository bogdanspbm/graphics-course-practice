//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#include "Placeable.h"
#include "utils/ModelUtils.hpp"


void Placeable::draw() {
    float modelMatrix[16];
    this->calcModelMatrix(modelMatrix);
    program->setUniformMatrix4FV("model", modelMatrix, true);
    Renderable::draw();
}

void Placeable::draw(ProgramAdapter* adapter) {
    program = adapter;
    float modelMatrix[16];
    this->calcModelMatrix(modelMatrix);
    program->setUniformMatrix4FV("model", modelMatrix, true);
    Renderable::draw();
}

void Placeable::addTexture(Texture *texture) {
    Renderable::addTexture(texture);
}

void Placeable::calcModelMatrix(float modelMatrix[16]) {
    // Identity matrix to start with
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

Renderable *Placeable::getModel() {
    return this;
}


std::vector<Vertex> *Placeable::getVertices() {
    return Renderable::getVertices();
}

std::vector<std::uint32_t> *Placeable::getIndices() {
    return Renderable::getIndices();
}

Placeable::Placeable(ProgramAdapter *programAdapter, const std::filesystem::path &path) : Renderable(programAdapter,
                                                                                                     path) {
    this->program = programAdapter;
    fillModelFromFile(this, path);
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
}

Placeable::Placeable() {

}

void Placeable::setPosition(Vector3F position) {
    this->position = position;
}

void Placeable::setRotation(Vector3F rotation) {
    this->rotation = rotation;
}

void Placeable::setScale(Vector3F scale) {
    this->scale = scale;
}

void Placeable::createVAO() {
    Renderable::createVAO();
}

void Placeable::bindVAO() {
    Renderable::bindVAO();
}

void Placeable::createVBO() {
    Renderable::createVBO();
}

void Placeable::bindVBO() {
    Renderable::bindVBO();
}

void Placeable::updateVBO() {
    Renderable::updateVBO();
}

void Placeable::createEBO() {
    Renderable::createEBO();
}

void Placeable::bindEBO() {
    Renderable::bindEBO();
}

void Placeable::updateEBO() {
    Renderable::updateEBO();
}

void Placeable::detachBuffers() {
    Renderable::detachBuffers();
}

void Placeable::setProgram(ProgramAdapter *program) {
    this->program = program;
    Renderable::setProgram(program);
}

