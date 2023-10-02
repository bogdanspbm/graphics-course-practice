//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#include "Renderable.h"
#include "utils/ModelUtils.hpp"


void Renderable::draw() {
    float modelMatrix[16];
    this->calcModelMatrix(modelMatrix);
    program->setUniformMatrix4FV("model", modelMatrix);
    Model::draw();
}

void Renderable::calcModelMatrix(float modelMatrix[16]) {
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

Model *Renderable::getModel() {
    return this;
}

void Renderable::createVAO() {
    Model::createVAO();
}

void Renderable::createVBO() {
    Model::createVBO();
}

void Renderable::createEBO() {
    Model::createEBO();
}

void Renderable::detachBuffers() {
    Model::detachBuffers();
}

std::vector<Vertex> *Renderable::getVertices() {
    return Model::getVertices();
}

std::vector<std::uint32_t> *Renderable::getIndices() {
    return Model::getIndices();
}

Renderable::Renderable(ProgramAdapter *programAdapter, const std::filesystem::path &path) : Model(programAdapter,
                                                                                                  path) {
    this->program = programAdapter;
    fillModelFromFile(this, path);
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
}

Renderable::Renderable() {

}

void Renderable::setPosition(Vector3D position) {
    this->position = position;
}

void Renderable::setRotation(Vector3D rotation) {
    this->rotation = rotation;
}

void Renderable::setScale(Vector3D scale) {
    this->scale = scale;
}

