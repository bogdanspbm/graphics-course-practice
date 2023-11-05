//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef PRACTICE4_LANDSCAPE_H
#define PRACTICE4_LANDSCAPE_H


#include "Placeable.h"

class Landscape : Placeable {
private:
    std::function<float(float x, float y)> heightFunction;
    u_int32_t cells = 128;
    float functionScale = 0.01;

    Vector3F colorA = {0.98, 0.68, 0.08};
    Vector3F colorB = {0.22, 0.06, 0.52};

public:
    Landscape(ProgramAdapter *programAdapter, std::function<float(float x, float y)> function);

    void draw();

    void increaseCells() {
        cells *= 2;
        generateVertices();
        generateIndices();

        Placeable::bindVAO();
        Placeable::bindEBO();
        Placeable::updateEBO();
        Placeable::detachBuffers();
    }

    void decreaseCells() {
        cells /= 2;
        if (cells < 16) {
            cells = 16;
        }
        generateVertices();
        generateIndices();

        Placeable::bindVAO();
        Placeable::bindEBO();
        Placeable::updateEBO();
        Placeable::detachBuffers();
    }

    void setPosition(Vector3F position);

    void setRotation(Vector3F rotation);

    void setScale(Vector3F scale);

    void setFunctionScale(float functionScale);

    void setColors(Vector3F colorA, Vector3F colorB);

    void updateFunction(std::function<float(float x, float y)> function);

    std::vector<Vertex> *getVertices();

    std::vector<std::uint32_t> *getIndices();

private:
    void generateVertices();

    void generateIndices();

    u_int32_t vertexPositionToIndex(u_int32_t x, u_int32_t y);
};


#endif //PRACTICE4_LANDSCAPE_H
