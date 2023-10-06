//
// Created by Bogdan Madzhuga on 30.09.2023.
//

#include "Renderable.h"
#include "utils/ModelUtils.hpp"


std::vector<Vertex> *Renderable::getVertices() {
    return &vertices;
}

std::vector<std::uint32_t> *Renderable::getIndices() {
    return &indices;
}


void Renderable::createVAO() {
    glGenVertexArrays(1, &vao);
    bindVAO();
}

void Renderable::createVBO() {
    glGenBuffers(1, &vbo);
    bindVBO();
    updateVBO();
}

void Renderable::createEBO() {
    if (indices.empty()) {
        return;
    }

    glGenBuffers(1, &ebo);
    bindEBO();
    updateEBO();
}

void Renderable::detachBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderable::draw() {
    glBindVertexArray(vao);

    if (!indices.empty()) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    glBindVertexArray(0);
}

Renderable::Renderable(ProgramAdapter *programAdapter, const std::filesystem::path &path) {
    this->program = programAdapter;
    fillModelFromFile(this, path);
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
}

Renderable::Renderable() {

}

void Renderable::bindVAO() {
    glBindVertexArray(vao);
}

void Renderable::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Renderable::bindEBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Renderable::updateEBO() {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Renderable::updateVBO() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // ScreenPosition
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(std::array<float, 3>)));

    // Texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(std::array<float, 3>) * 2));


    // Color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) (sizeof(std::array<float, 3>) * 2 + (sizeof(std::array<float, 2>))));

}

