//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#include "Model.h"
#include "utils/ObjectUtils.hpp"

Model::Model() {

}

Model::Model(std::filesystem::path const &path) {
    fillModelFromFile(this, path);
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
}

std::vector<Vertex> *Model::getVertices() {
    return &vertices;
}

std::vector<std::uint32_t> *Model::getIndices() {
    return &indices;
}


void Model::createVAO() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void Model::createVBO() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(std::array<float, 3>)));

    // Texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(std::array<float, 3>) * 2));
}

void Model::createEBO() {
    if (indices.empty()) {
        return;
    }

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Model::detachBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (indices.empty()) {
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::draw() {
    glBindVertexArray(vao);

    if (!indices.empty()) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    glBindVertexArray(0);
}