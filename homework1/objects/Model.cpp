//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#include "Model.h"

std::vector<Vertex>* Model::getVertices() {
    return &vertices;
}

std::vector<std::uint32_t>* Model::getIndices() {
    return &indices;
}
