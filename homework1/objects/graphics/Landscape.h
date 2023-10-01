//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef PRACTICE4_LANDSCAPE_H
#define PRACTICE4_LANDSCAPE_H


#include "Model.h"

class Landscape : Model {
private:
    std::function<float(float x, float y)> heightFunction;
    u_int32_t cells = 128;

public:
    Landscape(std::function<float(float x, float y)> function);

    void draw();

private:
    void generateVertices();

    void generateIndices();

    u_int32_t vertexPositionToIndex(u_int32_t x, u_int32_t y);
};


#endif //PRACTICE4_LANDSCAPE_H
