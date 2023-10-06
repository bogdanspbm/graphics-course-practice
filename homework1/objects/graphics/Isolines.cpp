//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include <iostream>
#include "Isolines.h"
#include "utils/MathUtils.h"

Isolines::Isolines(Landscape *landscape, ProgramAdapter *programAdapter) {
    this->landscape = landscape;
    this->program = programAdapter;
    generateVertices();
    generateIndices();
    Placeable::createVAO();
    Placeable::createVBO();
    Placeable::createEBO();
    Placeable::detachBuffers();
}

void Isolines::draw() {
    float modelMatrix[16];
    Placeable::calcModelMatrix(modelMatrix);
    program->setUniformMatrix4FV("model", modelMatrix);

    Placeable::bindVAO();

    if (!Placeable::getIndices()->empty()) {
        glDrawElements(GL_LINES, getIndices()->size(), GL_UNSIGNED_INT, 0);
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }


}

std::vector<Polygon> Isolines::getPolygons() {
    std::vector<Polygon> polygons;
    for (u_int32_t i = 0; i < landscape->getIndices()->size(); i += 3) {
        Polygon polygon = Polygon();
        auto a = (*landscape->getVertices())[(*landscape->getIndices())[i]];
        auto b = (*landscape->getVertices())[(*landscape->getIndices())[i + 1]];
        auto c = (*landscape->getVertices())[(*landscape->getIndices())[i + 2]];
        polygon.vertices = {a, b, c};
        polygons.push_back(polygon);
    }
    return polygons;
}

bool Isolines::hasIsoline(Polygon polygon, float z) {
    float a = polygon.vertices[0].position.z - z;
    float b = polygon.vertices[1].position.z - z;
    float c = polygon.vertices[2].position.z - z;

    if (a * b <= 0 || a * c <= 0 || c * b <= 0) {
        return true;
    }

    return false;
}

void Isolines::generateVertices() {
    float isolineH = 0.1f;
    Placeable::getVertices()->clear();
    Placeable::getIndices()->clear();

    std::unordered_map<std::string, int> map;

    for (auto polygon: this->getPolygons()) {
        auto flag = this->hasIsoline(polygon, isolineH);
        if (!flag) {
            continue;
        }
        auto vertices = getIsolineVertices(polygon, isolineH);
        if (vertices.size() == 2) {
            if (!map.contains(vertices[0].vertexToString())) {
                Placeable::getVertices()->push_back(vertices[0]);
                map[vertices[0].vertexToString()] = Placeable::getVertices()->size() - 1;
            }
            if (!map.contains(vertices[1].vertexToString())) {
                Placeable::getVertices()->push_back(vertices[1]);
                map[vertices[1].vertexToString()] = Placeable::getVertices()->size() - 1;
            }

            Placeable::getIndices()->push_back(map[vertices[0].vertexToString()]);
            Placeable::getIndices()->push_back(map[vertices[1].vertexToString()]);
        }
    }
}

std::vector<Vertex> Isolines::getIsolineVertices(Polygon polygon, float z) {
    std::vector<Vertex> vertices;

    float a = polygon.vertices[0].position.z - z;
    float b = polygon.vertices[1].position.z - z;
    float c = polygon.vertices[2].position.z - z;

    if (a == b && a == 0) {
        vertices.push_back(polygon.vertices[0]);
        vertices.push_back(polygon.vertices[1]);
    } else if (a * b <= 0) {
        float t = a / (a - b);
        Vertex v = Vertex();
        Vector3D direction = (polygon.vertices[1].position - polygon.vertices[0].position);
        Vector3D offset = direction * t;
        v.position = offset + polygon.vertices[0].position;
        vertices.push_back(v);
    }

    if (a == c && a == 0) {
        vertices.push_back(polygon.vertices[0]);
        vertices.push_back(polygon.vertices[2]);
    }
    if (a * c <= 0) {
        float t = a / (a - c);
        Vertex v = Vertex();
        v.position = (polygon.vertices[2].position - polygon.vertices[0].position) * t + polygon.vertices[0].position;
        vertices.push_back(v);
    }

    if (c == b && b == 0) {
        vertices.push_back(polygon.vertices[1]);
        vertices.push_back(polygon.vertices[2]);
    }
    if (b * c <= 0) {
        float t = b / (b - c);
        Vertex v = Vertex();
        v.position = (polygon.vertices[2].position - polygon.vertices[1].position) * t + polygon.vertices[1].position;
        vertices.push_back(v);
    }

    return vertices;
}


void Isolines::generateIndices() {
}

u_int32_t Isolines::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}

void Isolines::setColors(Vector3D colorA, Vector3D colorB) {
    this->colorA = colorA;
}

void Isolines::setPosition(Vector3D position) {
    Placeable::setPosition(position);
}

void Isolines::setRotation(Vector3D rotation) {
    Placeable::setRotation(rotation);
}

void Isolines::setScale(Vector3D scale) {
    Placeable::setScale(scale);
}

void Isolines::setFunctionScale(float functionScale) {
    this->functionScale = functionScale;
}

void Isolines::updateFunction() {
    generateVertices();
    Placeable::bindVAO();

    Placeable::bindVBO();
    Placeable::updateVBO();

    Placeable::bindEBO();
    Placeable::updateEBO();

    Placeable::detachBuffers();
}
