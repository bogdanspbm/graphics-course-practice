//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Landscape.h"
#include "utils/MathUtils.h"

Landscape::Landscape(ProgramAdapter *programAdapter, std::function<float(float, float)> function) {

    this->program = programAdapter;
    this->heightFunction = function;
    generateVertices();
    generateIndices();
    Placeable::createVAO();
    Placeable::createVBO();
    Placeable::createEBO();
    Placeable::detachBuffers();
}

void Landscape::draw() {
    Placeable::draw();
}

void Landscape::generateVertices() {
    Placeable::getVertices()->clear();
    for (int i = 0; i <= cells; i++) {
        for (int k = 0; k <= cells; k++) {
            Vertex vertex = Vertex();
            float x = ((float) i - (float) cells / 2);
            float y = ((float) k - (float) cells / 2);
            float z = heightFunction(x * functionScale, y * functionScale);
            vertex.position = {x / cells, y / cells, z};
            vertex.normal = positionToNormal(vertex.position);
            vertex.texcoord = {0.f, 0.f};
            auto colorInterpolation = linearInterpolation(colorA, colorB, z);
            vertex.color = {colorInterpolation.x, colorInterpolation.y, colorInterpolation.z};
            Placeable::getVertices()->push_back(vertex);
        }
    }
}

void Landscape::generateIndices() {
    Placeable::getIndices()->clear();
    for (u_int32_t i = 0; i < cells; i++) {
        for (u_int32_t k = 0; k < cells; k++) {
            auto a = vertexPositionToIndex(i, k);
            auto b = vertexPositionToIndex(i, k + 1);
            auto c = vertexPositionToIndex(i + 1, k + 1);
            auto d = vertexPositionToIndex(i + 1, k);

            // Polygon A
            Placeable::getIndices()->push_back(a);
            Placeable::getIndices()->push_back(b);
            Placeable::getIndices()->push_back(c);
            // Polygon B
            Placeable::getIndices()->push_back(a);
            Placeable::getIndices()->push_back(c);
            Placeable::getIndices()->push_back(d);
        }
    }
}

u_int32_t Landscape::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}

void Landscape::setColors(Vector3F colorA, Vector3F colorB) {
    this->colorA = colorA;
    this->colorB = colorB;
}

void Landscape::setPosition(Vector3F position) {
    Placeable::setPosition(position);
}

void Landscape::setRotation(Vector3F rotation) {
    Placeable::setRotation(rotation);
}

void Landscape::setScale(Vector3F scale) {
    Placeable::setScale(scale);
}

void Landscape::setFunctionScale(float functionScale) {
    this->functionScale = functionScale;
}

void Landscape::updateFunction(std::function<float(float, float)> function) {
    this->heightFunction = function;
    generateVertices();
    Placeable::bindVAO();
    Placeable::bindVBO();
    Placeable::updateVBO();
    Placeable::detachBuffers();
}

std::vector<std::uint32_t> *Landscape::getIndices() {
    return Placeable::getIndices();
}

std::vector<Vertex> *Landscape::getVertices() {
    return Placeable::getVertices();
}
