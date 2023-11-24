//
// Created by Bogdan Madzhuga on 23.09.2023.
//

#ifndef PRACTICE3_VERTEX_H
#define PRACTICE3_VERTEX_H

#include "array"
#include "string"
#include "utils/MathUtils.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class Vertex {
public:
    glm::vec3 position = {0, 0, 0};
    glm::vec3 normal = {0, 0, 1};
    glm::vec3 tangent = {0, 0, 1};
    glm::vec2 textCoord = {0, 0};
    glm::vec3 color = {1, 1, 1};

    std::string vertexToString() {
        return "(" + std::to_string(position.x) + ", " +
               std::to_string(position.y) + ", " + std::to_string(position.z) + ")";
    }

    void calculateNormal(const glm::vec3& edge1, const glm::vec3& edge2) {
        normal = cross(edge1,edge2);
    }

    // Calculate the tangent for this vertex
    void calculateTangent(const glm::vec3& edge1, const glm::vec3& edge2, const glm::vec2& deltaUV1, const glm::vec2& deltaUV2) {
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        this->tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        this->tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        this->tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    }

};


#endif //PRACTICE3_VERTEX_H
