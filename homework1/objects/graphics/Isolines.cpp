//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Isolines.h"
#include "utils/MathUtils.h"

Isolines::Isolines(ProgramAdapter *programAdapter, std::function<float(float, float)> function) {

    this->program = programAdapter;
    this->heightFunction = function;
    generateVertices();
    generateIndices();
    Placeable::createVAO();
    Placeable::createVBO();
    Placeable::createEBO();
    Placeable::detachBuffers();
}

void Isolines::draw() {
    Placeable::draw();
}

std::vector<Polygon> Isolines::getPolygons() {
    std::vector<Polygon> polygons;
    for (u_int32_t i = 0; i < Placeable::getIndices()->size(); i += 3) {
        Polygon polygon = Polygon();
        auto a = (*Placeable::getVertices())[(*Placeable::getIndices())[i]];
        auto b = (*Placeable::getVertices())[(*Placeable::getIndices())[i + 1]];
        auto c = (*Placeable::getVertices())[(*Placeable::getIndices())[i + 2]];
        polygon.vertices = {a, b, c};
        polygons.push_back(polygon);
    }
    return polygons;
}

bool Isolines::hasIsoline(Polygon polygon, float z) {
    float a = polygon.vertices[0].position.z - z;
    float b = polygon.vertices[1].position.z - z;
    float c = polygon.vertices[2].position.z - z;

    if (a * b <= 0 || a * c <= 0 || c * b <= 0) {
        return true;
    }

    return false;
}

void Isolines::generateVertices() {
    Placeable::getVertices()->clear();
    Placeable::getIndices()->clear();
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

void Isolines::generateIndices() {
}

u_int32_t Isolines::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}

void Isolines::setColors(Vector3D colorA, Vector3D colorB) {
    this->colorA = colorA;
    this->colorB = colorB;
}

void Isolines::setPosition(Vector3D position) {
    Placeable::setPosition(position);
}

void Isolines::setRotation(Vector3D rotation) {
    Placeable::setRotation(rotation);
}

void Isolines::setScale(Vector3D scale) {
    Placeable::setScale(scale);
}

void Isolines::setFunctionScale(float functionScale) {
    this->functionScale = functionScale;
}

void Isolines::updateFunction(std::function<float(float, float)> function) {
    this->heightFunction = function;
    generateVertices();
    generateIndices();
    Placeable::bindVAO();
    Placeable::bindVBO();
    Placeable::updateVBO();
    Placeable::detachBuffers();
}
