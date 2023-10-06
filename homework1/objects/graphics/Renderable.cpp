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
    glGenBuffers(1, &ebo);
    bindEBO();
    updateEBO();
}

void Renderable::detachBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (indices.empty()) {
        return;
    }
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
    if (indices.empty()) {
        return;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
}

void Renderable::updateVBO() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    // Attribute locations (you can define these as constants)
    const GLuint positionLocation = 0;
    const GLuint normalLocation = 1;
    const GLuint texcoordLocation = 2;
    const GLuint colorLocation = 3;

    // Vertex attribute pointers
    // Screen Position (Attribute 0)
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    // Normal (Attribute 1)
    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

    // Texture Coordinates (Attribute 2)
    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texcoord));

    // Color (Attribute 3)
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
}

