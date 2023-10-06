//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef PRACTICE4_ISOLINES_H
#define PRACTICE4_ISOLINES_H

#include "Placeable.h"
#include "structures/Polygon.h"
#include "Landscape.h"

class Isolines : Placeable {
private:
    u_int32_t cells = 300;
    float functionScale = 0.01;

    Vector3D colorA = {1, 1, 1};
    Landscape *landscape;

public:
    Isolines(Landscape *landscape, ProgramAdapter *programAdapter);

    void draw();

    void setPosition(Vector3D position);

    void setRotation(Vector3D rotation);

    void setScale(Vector3D scale);

    void setFunctionScale(float functionScale);

    void setColors(Vector3D colorA, Vector3D colorB);

    void updateFunction();

private:
    void generateVertices();

    void generateIndices();

    std::vector<Polygon> getPolygons();

    bool hasIsoline(Polygon polygon, float z);

    std::vector<Vertex> getIsolineVertices(Polygon polygon, float z);


    u_int32_t vertexPositionToIndex(u_int32_t x, u_int32_t y);
};


#endif //PRACTICE4_ISOLINES_H
