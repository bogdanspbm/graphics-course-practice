//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#ifndef PRACTICE4_MODEL_H
#define PRACTICE4_MODEL_H

#include "vector"
#include "structures/Vertex.h"

class Model {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;


public:
    std::vector<Vertex> getVertices();

    std::vector<std::uint32_t> getIndices();
};


#endif //PRACTICE4_MODEL_H
