//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#ifndef HOMEWORK1_PLACEABLE_H
#define HOMEWORK1_PLACEABLE_H


#include "Renderable.h"
#include "objects/opengl/ProgramAdapter.h"

class Placeable : Renderable {
private:
    Vector3D position = Vector3D();
    Vector3D rotation = Vector3D();
    Vector3D scale = Vector3D();
protected:
    ProgramAdapter *program;

protected:
    Placeable();

public:
    Placeable(ProgramAdapter *programAdapter, std::filesystem::path const &path);

    virtual void setPosition(Vector3D position);

    virtual void setRotation(Vector3D rotation);

    virtual void setScale(Vector3D scale);

    void draw() override;

protected:
    void createVAO() override;

    void bindVAO() override;

    void createVBO() override;

    void bindVBO() override;

    void updateVBO() override;

    void createEBO() override;

    void bindEBO() override;

    void updateEBO() override;

    void detachBuffers() override;

    void setProgram(ProgramAdapter *program);

public:

    std::vector<Vertex> *getVertices() override;

    std::vector<std::uint32_t> *getIndices();

    Renderable *getModel();

    void addTexture(Texture *texture);

    void calcModelMatrix(float modelMatrix[16]);

};


#endif //HOMEWORK1_PLACEABLE_H
