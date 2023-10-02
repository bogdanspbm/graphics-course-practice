//
// Created by Bogdan Madzhuga on 23.09.2023.
//

#ifndef PRACTICE3_VERTEX_H
#define PRACTICE3_VERTEX_H

#include "array"

struct Vertex {
    std::array<float, 3> position;
    std::array<float, 3> normal;
    std::array<float, 2> texcoord;
    std::array<float, 3> color;
};



#endif //PRACTICE3_VERTEX_H
