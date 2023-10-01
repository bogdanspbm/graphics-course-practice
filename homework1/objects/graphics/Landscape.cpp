//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Landscape.h"
#include "utils/MathUtils.h"

Landscape::Landscape(std::function<float(float, float)> function) {
    this->heightFunction = function;
    generateVertices();
    generateIndices();
    Model::createVAO();
    Model::createVBO();
    Model::createEBO();
    Model::detachBuffers();
}

void Landscape::draw() {
    Model::draw();
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
            vertices.push_back(vertex);
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
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
            // Polygon B
            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
}

u_int32_t Landscape::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}