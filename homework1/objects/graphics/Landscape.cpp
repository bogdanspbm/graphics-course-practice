//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Landscape.h"

Landscape::Landscape(std::function<float(float, float)> function) {
    this->heightFunction = function;
    generateVertices();
    generateIndices();
}

void Landscape::draw() {
    Model::draw();
}

void Landscape::generateVertices() {
    for (u_int32_t i = 0; i <= cells; i++) {
        for (u_int32_t k = 0; k <= cells; i++) {
            Vertex vertex = Vertex();
            float x = i;
            float y = i;
            float z = heightFunction(x, y);
            vertex.position = {x, y, z};
            vertices.push_back(vertex);
        }
    }
}

void Landscape::generateIndices() {
    for (u_int32_t i = 0; i < cells; i++) {
        for (u_int32_t k = 0; k < cells; i++) {
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