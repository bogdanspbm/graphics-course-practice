//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_RENDERABLE_H
#define HOMEWORK2_RENDERABLE_H


#include <GL/glew.h>
#include <vector>
#include "filesystem"
#include "structures/Vertex.h"
#include "objects/graphics/textures/Material.h"
#include "glm/geometric.hpp"
#include "structures/Particle.h"
#include <map>

class Renderable {

private:

    static std::map<std::string, Renderable *> cachedRenderable;

    // Identifier
    std::string path = "";
    std::string name = "";
    std::string materialName = "";

    std::vector<Particle> particles;
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    Renderable(std::filesystem::path const &path);

    Renderable();

public:

    Renderable(std::vector<Particle> particles);

public:
    static Renderable *getRenderableByPath(std::filesystem::path const &path) {
        if (cachedRenderable.contains(path)) {
            return cachedRenderable[path];
        }

        Renderable *renderable = new Renderable(path);
        cachedRenderable[path] = renderable;
        return renderable;
    }

    static Renderable *getRenderableByName(std::string const name) {
        if (cachedRenderable.contains(name)) {
            return cachedRenderable[name];
        }

        Renderable *renderable = new Renderable();
        cachedRenderable[name] = renderable;
        renderable->name = name;
        return renderable;
    }

    void calculateNormalsAndTangents() {
        // Initialize normals and tangents to zero for all vertices
        for (Vertex &vertex: vertices) {
            vertex.normal = glm::vec3(0.0f);
            vertex.tangent = glm::vec3(0.0f);
        }

        // Calculate normals and tangents for each triangle
        for (size_t i = 0; i < indices.size(); i += 3) {
            const glm::vec3 &v0 = vertices[indices[i]].position;
            const glm::vec3 &v1 = vertices[indices[i + 1]].position;
            const glm::vec3 &v2 = vertices[indices[i + 2]].position;

            const glm::vec2 &uv0 = vertices[indices[i]].textCoord;
            const glm::vec2 &uv1 = vertices[indices[i + 1]].textCoord;
            const glm::vec2 &uv2 = vertices[indices[i + 2]].textCoord;

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec2 deltaUV1 = uv1 - uv0;
            glm::vec2 deltaUV2 = uv2 - uv0;

            // Calculate the triangle normal

            // Calculate tangents and accumulate normals for each vertex of the triangle
            for (int j = 0; j < 3; ++j) {
                vertices[indices[i + j]].calculateTangent(edge1, edge2, deltaUV1, deltaUV2);
                vertices[indices[i + j]].calculateNormal(edge1, edge2);
            }
        }

        // Normalize the accumulated normals and tangents for each vertex
        for (Vertex &vertex: vertices) {
            vertex.normal = glm::normalize(vertex.normal);
            vertex.tangent = glm::normalize(vertex.tangent);
        }
    }


protected:

std::string getMaterialName() {
    if (materialName != "") {
        return materialName;
    }

    if (name != "") {
        return name;
    }

    if (path != "") {
        return path;
    }

    return path;
}

public:
// Methods

virtual void generateBuffers()

final;

virtual void draw()

final;

virtual void createVAO()

final;

virtual void bindVAO()

final;

virtual void createVBO()

final;

virtual void bindVBO()

final;

virtual void updateVBO()

final;

virtual void createEBO()

final;

virtual void bindEBO()

final;

virtual void updateEBO()

final;

virtual void detachBuffers()

final;

public:

// Getters
virtual std::vector<Vertex> *getVertices()

final;

virtual std::vector<std::uint32_t> *getIndices()

final;

virtual Material *getMaterial()

final;

virtual void setMaterialName(std::string name);

virtual void setPath(std::string path);

virtual std::string getName();

virtual bool isParticle() final;

virtual void setParticles(std::vector<Particle> particles) final;

};


#endif //HOMEWORK2_RENDERABLE_H
