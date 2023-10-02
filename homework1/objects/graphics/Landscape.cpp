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
    Renderable::createVAO();
    Renderable::createVBO();
    Renderable::createEBO();
    Renderable::detachBuffers();
}

void Landscape::draw() {
    Renderable::draw();
}

void Landscape::generateVertices() {
    for (int i = 0; i <= cells; i++) {
        for (int k = 0; k <= cells; k++) {
            Vertex vertex = Vertex();
            float x = (float) i - (float) cells / 2;
            float y = (float) k - (float) cells / 2;
            float z = heightFunction(x, y);
            vertex.position = {x / cells, y / cells, z / cells};
            vertex.normal = positionToNormal(vertex.position);
            vertex.texcoord = {0.f, 0.f};
            Renderable::getVertices()->push_back(vertex);
        }
    }
}

void Landscape::generateIndices() {
    for (u_int32_t i = 0; i < cells; i++) {
        for (u_int32_t k = 0; k < cells; k++) {
            auto a = vertexPositionToIndex(i, k);
            auto b = vertexPositionToIndex(i, k + 1);
            auto c = vertexPositionToIndex(i + 1, k + 1);
            auto d = vertexPositionToIndex(i + 1, k);

            // Polygon A
            Renderable::getIndices()->push_back(a);
            Renderable::getIndices()->push_back(b);
            Renderable::getIndices()->push_back(c);
            // Polygon B
            Renderable::getIndices()->push_back(a);
            Renderable::getIndices()->push_back(c);
            Renderable::getIndices()->push_back(d);
        }
    }
}

u_int32_t Landscape::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}

void Landscape::setPosition(Vector3D position) {
    Renderable::setPosition(position);
}

void Landscape::setRotation(Vector3D rotation) {
    Renderable::setRotation(rotation);
}

void Landscape::setScale(Vector3D scale) {
    Renderable::setScale(scale);
}
