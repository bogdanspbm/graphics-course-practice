//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#ifndef HOMEWORK1_PLACEABLE_H
#define HOMEWORK1_PLACEABLE_H


#include "Renderable.h"
#include "objects/opengl/ProgramAdapter.h"

class Placeable : Renderable {
private:
    Vector3F position = Vector3F();
    Vector3F rotation = Vector3F();
    Vector3F scale = Vector3F();
protected:
    ProgramAdapter *program;

protected:
    Placeable();

public:
    Placeable(ProgramAdapter *programAdapter, std::filesystem::path const &path);

    virtual void setPosition(Vector3F position);

    virtual void setRotation(Vector3F rotation);

    virtual void setScale(Vector3F scale);

    void draw() override;

    void draw(ProgramAdapter *adapter);

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
