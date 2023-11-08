//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_PLACEABLE_H
#define HOMEWORK2_PLACEABLE_H

#include <string>
#include <filesystem>
#include "objects/opengl/GLProgram.h"
#include "Renderable.h"


class Placeable {
private:

    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};

    // Identifier
    std::string path = "";
    Renderable *renderable;

public:
    Placeable(std::filesystem::path const &path);

private:
    void calcModelMatrix(float modelMatrix[16]);

public:
    // Methods
    void draw();

    virtual void setPosition(glm::vec3 position);

    virtual void setRotation(glm::vec3 rotation);

    virtual void setScale(glm::vec3 scale);

    virtual Material *getMaterial() final;
};


#endif //HOMEWORK2_PLACEABLE_H
