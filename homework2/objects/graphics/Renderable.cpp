//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "Renderable.h"
#include "utils/ObjectUtils.h"

std::map<std::string, Renderable *> Renderable::cachedRenderable;

std::vector<Vertex> *Renderable::getVertices() {
    return &this->vertices;
}

std::vector<std::uint32_t> *Renderable::getIndices() {
    return &this->indices;
}


Renderable::Renderable(const std::filesystem::path &path) {
    this->path = path;
    fillRenderableFromFile(this, path);
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
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
    bindVAO();

    bindTextures();

    if (!indices.empty()) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    detachBuffers();
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

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, textCoord));

    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
}

void Renderable::bindTextures() {
}

