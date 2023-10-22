//
// Created by Bogdan Madzhuga on 21.10.2023.
//

#include "Plane.h"

Plane::Plane(ProgramAdapter *programAdapter) {
    this->setProgram(programAdapter);

    Vertex v1, v2, v3, v4;

    v1.position = {-0.5f, -0.5f, 0.0f};  // Bottom-left corner
    v1.texcoord = {0.0f, 0.0f};          // Texture coordinate

    // Vertex 2
    v2.position = {0.5f, -0.5f, 0.0f};   // Bottom-right corner
    v2.texcoord = {1.0f, 0.0f};          // Texture coordinate

    // Vertex 3
    v3.position = {-0.5f, 0.5f, 0.0f};   // Top-left corner
    v3.texcoord = {0.0f, 1.0f};          // Texture coordinate

    // Vertex 4
    v4.position = {0.5f, 0.5f, 0.0f};    // Top-right corner
    v4.texcoord = {1.0f, 1.0f};          // Texture coordinate

    // Add the vertices to the plane's vertices vector
    this->getVertices()->push_back(v1);
    this->getVertices()->push_back(v2);
    this->getVertices()->push_back(v3);
    this->getVertices()->push_back(v4);

    this->getIndices()->push_back(0); // Index of v1
    this->getIndices()->push_back(1); // Index of v2
    this->getIndices()->push_back(2); // Index of v3

    // Triangle 2 (v2, v4, v3)
    this->getIndices()->push_back(1); // Index of v2
    this->getIndices()->push_back(3); // Index of v4
    this->getIndices()->push_back(2); // Index of v3

    Placeable::createVAO();
    Placeable::createVBO();
    Placeable::createEBO();
    Placeable::detachBuffers();
}

void Plane::draw() {
    Placeable::draw();
}

void Plane::setProgram(ProgramAdapter *program) {
    Placeable::setProgram(program);
}

void Plane::setPosition(Vector3D position) {
    Placeable::setPosition(position);
}

void Plane::setRotation(Vector3D rotation) {
    Placeable::setRotation(rotation);
}

void Plane::setScale(Vector3D scale) {
    Placeable::setScale(scale);
}
