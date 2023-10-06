//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef PRACTICE4_LANDSCAPE_H
#define PRACTICE4_LANDSCAPE_H


#include "Placeable.h"
#include "structures/Polygon.h"

class Isolines : Placeable {
private:
    std::function<float(float x, float y)> heightFunction;
    u_int32_t cells = 300;
    float functionScale = 0.01;

    Vector3D colorA = {0.98, 0.68, 0.08};
    Vector3D colorB = {0.22, 0.06, 0.52};

public:
    Isolines(ProgramAdapter *programAdapter, std::function<float(float x, float y)> function);

    void draw();

    void setPosition(Vector3D position);

    void setRotation(Vector3D rotation);

    void setScale(Vector3D scale);

    void setFunctionScale(float functionScale);

    void setColors(Vector3D colorA, Vector3D colorB);

    void updateFunction(std::function<float(float x, float y)> function);

private:
    void generateVertices();

    void generateIndices();

    std::vector<Polygon> getPolygons();

    bool hasIsoline(Polygon polygon, float z);


    u_int32_t vertexPositionToIndex(u_int32_t x, u_int32_t y);
};


#endif //PRACTICE4_LANDSCAPE_H
