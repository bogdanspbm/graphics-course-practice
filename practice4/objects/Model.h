//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#ifndef PRACTICE4_MODEL_H
#define PRACTICE4_MODEL_H

#include <GL/glew.h>
#include "vector"
#include "structures/Vertex.h"
#include "filesystem"

class Model {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

public:
    Model();

    Model(std::filesystem::path const &path);

private:
    void createVAO();

    void createVBO();

    void createEBO();

    void detachBuffers();

public:
    std::vector<Vertex> *getVertices();

    std::vector<std::uint32_t> *getIndices();

    void draw();
};


#endif //PRACTICE4_MODEL_H
