//
// Created by Bogdan Madzhuga on 23.09.2023.
//

#ifndef PRACTICE3_VERTEX_H
#define PRACTICE3_VERTEX_H

#include "array"
#include "string"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class Vertex {
public:
    glm::vec3 position = {0, 0, 0};
    glm::vec3 normal = {0, 0, 1};
    glm::vec2 textCoord = {0, 0};
    glm::vec3 color = {1, 1, 1};

    std::string vertexToString() {
        return "(" + std::to_string(position.x) + ", " +
               std::to_string(position.y) + ", " + std::to_string(position.z) + ")";
    }
};


#endif //PRACTICE3_VERTEX_H
