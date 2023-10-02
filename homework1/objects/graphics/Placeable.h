//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#ifndef HOMEWORK1_PLACEABLE_H
#define HOMEWORK1_PLACEABLE_H


#include "Renderable.h"
#include "structures/Vector.h"
#include "objects/opengl/ProgramAdapter.h"

class Placeable : Renderable {
private:
    Vector3D position = Vector3D();
    Vector3D rotation = Vector3D();
    Vector3D scale = Vector3D();
protected:
    ProgramAdapter *program;

protected:
    void createVAO();

    void createVBO();

    void createEBO();

    void detachBuffers();

protected:
    Placeable();

public:

    void setPosition(Vector3D position);

    void setRotation(Vector3D rotation);

    void setScale(Vector3D scale);

    Placeable(ProgramAdapter *programAdapter, std::filesystem::path const &path);

    void draw();

    std::vector<Vertex> *getVertices();

    std::vector<std::uint32_t> *getIndices();

    Renderable *getModel();

private:
    void calcModelMatrix(float modelMatrix[16]);
};


#endif //HOMEWORK1_PLACEABLE_H
