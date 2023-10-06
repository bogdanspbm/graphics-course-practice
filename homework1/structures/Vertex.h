//
// Created by Bogdan Madzhuga on 23.09.2023.
//

#ifndef PRACTICE3_VERTEX_H
#define PRACTICE3_VERTEX_H

#include "array"
#include "Vector3D.h"

struct Vertex {
    Vector3D position = {0, 0, 0};
    Vector3D normal = {0, 0, 1};
    std::array<float, 2> texcoord = {0, 0};
    Vector3D color = {1, 1, 1};
};


#endif //PRACTICE3_VERTEX_H
