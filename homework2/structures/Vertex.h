//
// Created by Bogdan Madzhuga on 23.09.2023.
//

#ifndef PRACTICE3_VERTEX_H
#define PRACTICE3_VERTEX_H

#include "array"
#include "string"
#include "Vector3F.h"

class Vertex {
public:
    Vector3F position = {0, 0, 0};
    Vector3F normal = {0, 0, 1};
    std::array<float, 2> texcoord = {0, 0};
    Vector3F color = {1, 1, 1};

    std::string vertexToString() {
        return "(" + std::to_string(position.x) + ", " +
               std::to_string(position.y) + ", " + std::to_string(position.z) + ")";
    }
};


#endif //PRACTICE3_VERTEX_H
