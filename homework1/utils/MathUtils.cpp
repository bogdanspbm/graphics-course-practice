//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include <array>
#include "MathUtils.h"

std::array<float, 3> positionToNormal(std::array<float, 3> position) {
    float length = std::sqrt(position[0] * position[0] + position[1] * position[1] + position[2] * position[2]);
    if (length < 1e-6) {
        return {0.0f, 0.0f, 1.0f};
    }

    std::array<float, 3> normal = {
            -position[1] / length,
            position[0] / length,
            0.0f
    };

    return normal;
}