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

Material *Renderable::getMaterial() {
    return Material::getMaterialByName(this->getMaterialName());
}

Renderable::Renderable(const std::filesystem::path &path) {
    this->path = path;
    fillRenderableFromFile(this, path);
    generateBuffers();
}

Renderable::Renderable() {
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

    this->getMaterial()->bindMaterial();

    if (isParticle()) {
        glDrawArrays(GL_POINTS, 0, particles.size());
    } else if (!indices.empty()) {
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
    if (isParticle()) {
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_STATIC_DRAW);

        if (vgoGenerated) {
            return;
        }

        vgoGenerated = true;

        const GLuint positionLocation = 0;
        const GLuint rotationLocation = 1;
        const GLuint sizeLocation = 2;
        const GLuint colorPosition = 3;

        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
                              (void *) offsetof(Particle, position));

        glEnableVertexAttribArray(rotationLocation);
        glVertexAttribPointer(rotationLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
                              (void *) offsetof(Particle, rotation));

        glEnableVertexAttribArray(sizeLocation);
        glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, size));

        glEnableVertexAttribArray(colorPosition);
        glVertexAttribPointer(colorPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
                              (void *) offsetof(Particle, color));

    } else {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        if (vgoGenerated) {
            return;
        }

        vgoGenerated = true;

        // Attribute locations (you can define these as constants)
        const GLuint positionLocation = 0;
        const GLuint normalLocation = 1;
        const GLuint tangentLocation = 2;
        const GLuint texcoordLocation = 3;
        const GLuint colorLocation = 4;

        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, position));

        glEnableVertexAttribArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

        glEnableVertexAttribArray(tangentLocation);
        glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, tangent));


        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, textCoord));

        glEnableVertexAttribArray(colorLocation);
        glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
    }
}

Renderable::Renderable(std::vector<glm::vec3> lVertices, std::vector<std::uint32_t> lIndices) {
    for (int i = 0; i < lVertices.size(); i++) {
        glm::vec3 position = lVertices[i];
        auto vertex = Vertex();
        vertex.position = position;
        this->vertices.push_back(vertex);
    }

    for (int i = 0; i < lIndices.size(); i++) {
        std::uint32_t index = lIndices[i];
        this->indices.push_back(index);
    }

    this->generateBuffers();
}

void Renderable::generateBuffers() {
    createVAO();
    createVBO();
    createEBO();
    detachBuffers();
}

void Renderable::setMaterialName(std::string name) {
    this->materialName = name;
}

void Renderable::setPath(std::string path) {
    this->path = path;
}

std::string Renderable::getName() {
    return this->name;
}

bool Renderable::isParticle() {
    return particles.size() > 0;
}

Renderable::Renderable(std::vector<Particle> particles) {
    this->particles = particles;
    generateBuffers();
}

void Renderable::setParticles(std::vector<Particle> particles) {
    this->particles = particles;
    bindVBO();
    updateVBO();
}
