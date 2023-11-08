//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_RENDERABLE_H
#define HOMEWORK2_RENDERABLE_H


#include <GL/glew.h>
#include <vector>
#include "filesystem"
#include "structures/Vertex.h"
#include "Material.h"
#include <map>

class Renderable {

private:

    static std::map<std::string, Renderable *> cachedRenderable;

    // Identifier
    std::string path = "";

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    Material *material = new Material();

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    Renderable(std::filesystem::path const &path);

public:
    static Renderable *getRenderable(std::filesystem::path const &path) {
        if (cachedRenderable.contains(path)) {
            return cachedRenderable[path];
        }

        Renderable *renderable = new Renderable(path);
        cachedRenderable[path] = renderable;
        return renderable;
    }

public:
    // Methods
    virtual void draw() final;

    virtual void createVAO() final;

    virtual void bindVAO() final;

    virtual void createVBO() final;

    virtual void bindVBO() final;

    virtual void updateVBO() final;

    virtual void createEBO() final;

    virtual void bindEBO() final;

    virtual void updateEBO() final;

    virtual void detachBuffers() final;

public:
    // Getters
    virtual std::vector<Vertex> *getVertices() final;

    virtual std::vector<std::uint32_t> *getIndices() final;

    virtual Material *getMaterial() final;

};


#endif //HOMEWORK2_RENDERABLE_H
