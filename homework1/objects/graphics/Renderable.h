//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#ifndef PRACTICE4_MODEL_H
#define PRACTICE4_MODEL_H

#include <GL/glew.h>
#include "vector"
#include "structures/Vertex.h"
#include "filesystem"
#include "objects/opengl/ProgramAdapter.h"

class Renderable {

protected:
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    ProgramAdapter *program;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

protected:
    Renderable();

public:
    Renderable(ProgramAdapter *programAdapter, std::filesystem::path const &path);

protected:

    void createVAO();

    void createVBO();

    void createEBO();

    void detachBuffers();

public:

    std::vector<Vertex> *getVertices();

    std::vector<std::uint32_t> *getIndices();

    virtual void draw();
};


#endif //PRACTICE4_MODEL_H
