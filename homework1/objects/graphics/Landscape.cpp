//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Landscape.h"
#include "utils/MathUtils.h"

Landscape::Landscape(ProgramAdapter *programAdapter, std::function<float(float, float)> function) {

    this->program = programAdapter;
    this->heightFunction = function;
    generateVertices();
    generateIndices();

    Placeable::createVAO();
    Placeable::createVBO();
    Placeable::createEBO();
    Placeable::detachBuffers();

    generateIsoLines();
    createIsoLinesVAO();
    createIsoLinesVBO();
    createIsoLinesEBO();
    detachIsoLinesBuffers();
}

void Landscape::draw() {

    Placeable::draw();

    glBindVertexArray(isoLinesVao);

    if (!isoLinesVertices.empty()) {
        glDrawElements(GL_LINES, isoLinesVertices.size(), GL_UNSIGNED_INT, 0);
    } else if (!isoLinesVertices.empty()) {
        glDrawArrays(GL_LINES, 0, isoLinesVertices.size());
    }

    glBindVertexArray(0);
}

void Landscape::generateVertices() {
    Placeable::getVertices()->clear();
    for (int i = 0; i <= cells; i++) {
        for (int k = 0; k <= cells; k++) {
            Vertex vertex = Vertex();
            float x = ((float) i - (float) cells / 2);
            float y = ((float) k - (float) cells / 2);
            float z = heightFunction(x * functionScale, y * functionScale);
            vertex.position = {x / cells, y / cells, z};
            vertex.normal = positionToNormal(vertex.position);
            vertex.texcoord = {0.f, 0.f};
            auto colorInterpolation = linearInterpolation(colorA, colorB, z);
            vertex.color = {colorInterpolation.x, colorInterpolation.y, colorInterpolation.z};
            Placeable::getVertices()->push_back(vertex);
        }
    }
}

void Landscape::generateIndices() {
    Placeable::getIndices()->clear();
    for (u_int32_t i = 0; i < cells; i++) {
        for (u_int32_t k = 0; k < cells; k++) {
            auto a = vertexPositionToIndex(i, k);
            auto b = vertexPositionToIndex(i, k + 1);
            auto c = vertexPositionToIndex(i + 1, k + 1);
            auto d = vertexPositionToIndex(i + 1, k);

            // Polygon A
            Placeable::getIndices()->push_back(a);
            Placeable::getIndices()->push_back(b);
            Placeable::getIndices()->push_back(c);
            // Polygon B
            Placeable::getIndices()->push_back(a);
            Placeable::getIndices()->push_back(c);
            Placeable::getIndices()->push_back(d);
        }
    }
}

u_int32_t Landscape::vertexPositionToIndex(u_int32_t x, u_int32_t y) {
    return x + cells * y;
}

void Landscape::setColors(Vector3D colorA, Vector3D colorB) {
    this->colorA = colorA;
    this->colorB = colorB;
}

void Landscape::setPosition(Vector3D position) {
    Placeable::setPosition(position);
}

void Landscape::setRotation(Vector3D rotation) {
    Placeable::setRotation(rotation);
}

void Landscape::setScale(Vector3D scale) {
    Placeable::setScale(scale);
}

void Landscape::setFunctionScale(float functionScale) {
    this->functionScale = functionScale;
}

void Landscape::updateFunction(std::function<float(float, float)> function) {
    this->heightFunction = function;
    generateVertices();
    generateIndices();
    Placeable::bindVAO();

    Placeable::bindVBO();
    Placeable::updateVBO();

    Placeable::detachBuffers();


    generateIsoLines();

    bindIsoLinesVAO();

    bindIsoLinesVBO();
    updateIsoLinesVBO();

    bindIsoLinesEBO();
    updateIsoLinesEBO();

    detachIsoLinesBuffers();
}

void Landscape::generateIsoLines() {
    float isolineH = 0.1f;
    isoLinesVertices.clear();
    isoLinesIndices.clear();
    for (auto polygon: this->getPolygons()) {
        auto flag = this->hasIsoline(polygon, isolineH);
        if (!flag) {
            continue;
        }
        auto vertices = getIsolineVertices(polygon, isolineH);
        if (vertices.size() == 2) {
            isoLinesVertices.push_back(vertices[0]);
            isoLinesVertices.push_back(vertices[1]);
            isoLinesIndices.push_back(isoLinesVertices.size() - 2);
            isoLinesIndices.push_back(isoLinesVertices.size() - 1);
        }
    }
}

std::vector<Polygon> Landscape::getPolygons() {
    std::vector<Polygon> polygons;
    for (u_int32_t i = 0; i < Placeable::getIndices()->size(); i += 3) {
        Polygon polygon = Polygon();
        auto a = (*Placeable::getVertices())[(*Placeable::getIndices())[i]];
        auto b = (*Placeable::getVertices())[(*Placeable::getIndices())[i + 1]];
        auto c = (*Placeable::getVertices())[(*Placeable::getIndices())[i + 2]];
        polygon.vertices = {a, b, c};
        polygons.push_back(polygon);
    }
    return polygons;
}

bool Landscape::hasIsoline(Polygon polygon, float z) {
    float a = polygon.vertices[0].position.z - z;
    float b = polygon.vertices[1].position.z - z;
    float c = polygon.vertices[2].position.z - z;

    if (a * b <= 0 || a * c <= 0 || c * b <= 0) {
        return true;
    }

    return false;
}

std::vector<Vertex> Landscape::getIsolineVertices(Polygon polygon, float z) {
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

void Landscape::createIsoLinesVAO() {
    glGenVertexArrays(1, &isoLinesVao);
    bindIsoLinesVAO();
}

void Landscape::bindIsoLinesVAO() {
    glBindVertexArray(isoLinesVao);
    //printf("Bind: %u\n", isoLinesVao);
}

void Landscape::createIsoLinesVBO() {
    //printf("Create: %u\n", isoLinesVao);
    glGenVertexArrays(1, &isoLinesVbo);
    bindIsoLinesVBO();
    updateIsoLinesVBO();
}

void Landscape::bindIsoLinesVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, isoLinesVbo);
}

void Landscape::updateIsoLinesVBO() {

    //printf("Update: %u\n", isoLinesVao);

    if (isoLinesVertices.empty()) {
        return;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * isoLinesVertices.size(), isoLinesVertices.data(), GL_STATIC_DRAW);

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

void Landscape::createIsoLinesEBO() {
    glGenVertexArrays(1, &isoLinesEbo);
    bindIsoLinesEBO();
    updateIsoLinesEBO();
}

void Landscape::bindIsoLinesEBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, isoLinesEbo);
}

void Landscape::updateIsoLinesEBO() {

    if (isoLinesIndices.empty()) {
        return;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * isoLinesIndices.size(), isoLinesIndices.data(),
                 GL_STATIC_DRAW);

}

void Landscape::detachIsoLinesBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
