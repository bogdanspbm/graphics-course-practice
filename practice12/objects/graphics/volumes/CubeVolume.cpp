//
// Created by Bogdan Madzhuga on 08.12.2023.
//

#include "CubeVolume.h"

static std::vector<glm::vec3> cubeVertices
        {
                {0.f, 0.f, 0.f},
                {1.f, 0.f, 0.f},
                {0.f, 1.f, 0.f},
                {1.f, 1.f, 0.f},
                {0.f, 0.f, 1.f},
                {1.f, 0.f, 1.f},
                {0.f, 1.f, 1.f},
                {1.f, 1.f, 1.f},
        };

static std::vector<std::uint32_t> cubeIndices
        {
                // -Z
                0, 2, 1,
                1, 2, 3,
                // +Z
                4, 5, 6,
                6, 5, 7,
                // -Y
                0, 1, 4,
                4, 1, 5,
                // +Y
                2, 6, 3,
                3, 6, 7,
                // -X
                0, 4, 2,
                2, 4, 6,
                // +X
                1, 3, 5,
                5, 3, 7,
        };

CubeVolume::CubeVolume() {
    auto renderable = new Renderable(cubeVertices, cubeIndices);
    this->placeable = new Placeable(renderable);
}

Placeable *CubeVolume::getPlaceable() {
    return this->placeable;
}

void CubeVolume::draw() {
    GLProgram::getGLProgram()->setUniformVector3F("bBoxMin", boxMin);
    GLProgram::getGLProgram()->setUniformVector3F("bBoxMax", boxMax);
    this->placeable->draw();
}
