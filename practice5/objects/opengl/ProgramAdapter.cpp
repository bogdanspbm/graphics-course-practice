//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "ProgramAdapter.h"
#include "utils/MathUtils.h"

void ProgramAdapter::useProgram() {
    glUseProgram(this->id);
    setProjectionMatrix();
    setViewMatrix();
}

void ProgramAdapter::setUniformMatrix4FV(const GLchar *name, GLfloat *value, bool transpose) {
    GLint uniformLocationID = glGetUniformLocation(this->id, name);

    if (uniformLocationID == -1) {
        throw std::invalid_argument(TAG + ": Can't find uniform location.");
    } else {
        glUniformMatrix4fv(uniformLocationID, 1, transpose ? GL_TRUE : GL_FALSE, value);
    }
}

void ProgramAdapter::cleanUniformMatrix4FV(const GLchar *name) {
    float matrix[16] =
            {
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f,
            };
    this->setUniformMatrix4FV(name, matrix, true);
}

void ProgramAdapter::bindTexture(Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    int textureLocation = glGetUniformLocation(this->id, texture->getLayerName().c_str());
    if (textureLocation != -1) {
        glUniform1i(textureLocation, 0); // 0 corresponds to GL_TEXTURE0
    }
}

void ProgramAdapter::calcProjectionMatrix(float matrix[16]) {
    float right = near * tan(fov / 2.0f);
    float aspectRatio = width / height;
    float top = right / aspectRatio;

    memset(matrix, 0, sizeof(matrix));
    matrix[0] = (2.0f * near) / (right * 2.0f);
    matrix[1] = 0.f;
    matrix[2] = 0.f;
    matrix[3] = 0.f;

    matrix[4] = 0.f;
    matrix[5] = (2.0f * near) / (top * 2.0f);
    matrix[6] = 0.f;
    matrix[7] = 0.f;

    matrix[8] = 0.f;
    matrix[9] = 0.f;
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -(2.0f * far * near) / (far - near);

    matrix[12] = 0.f;
    matrix[13] = 0.f;
    matrix[14] = -1.f;
    matrix[15] = 0.f;
}

void ProgramAdapter::setProjectionMatrix() {
    float projection[16];
    calcProjectionMatrix(projection);
    setUniformMatrix4FV("projection", projection, true);
}

void ProgramAdapter::setViewMatrix() {
    float view[16];
    calcViewMatrix(view);
    setUniformMatrix4FV("view", view, false);
}

void ProgramAdapter::calcViewMatrix(float *matrix) {
    memset(matrix, 0, sizeof(float) * 16);
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;

    // Apply translation (camera position)
    matrix[3] = -position.x;
    matrix[7] = -position.y;
    matrix[11] = -position.z;

    // Apply rotation (camera rotation) - Assuming rotation is in degrees
    float radX = rotation.x * M_PI / 180.0f;
    float radY = rotation.y * M_PI / 180.0f;
    float radZ = rotation.z * M_PI / 180.0f;

    float cosX = cosf(radX);
    float sinX = sinf(radX);
    float cosY = cosf(radY);
    float sinY = sinf(radY);
    float cosZ = cosf(radZ);
    float sinZ = sinf(radZ);

    matrix[0] = cosY * cosZ;
    matrix[1] = cosX * sinZ + sinX * sinY * cosZ;
    matrix[2] = sinX * sinZ - cosX * sinY * cosZ;

    matrix[4] = -cosY * sinZ;
    matrix[5] = cosX * cosZ - sinX * sinY * sinZ;
    matrix[6] = sinX * cosZ + cosX * sinY * sinZ;

    matrix[8] = sinY;
    matrix[9] = -sinX * cosY;
    matrix[10] = cosX * cosY;
}

void ProgramAdapter::setResolution(float width, float height) {
    this->width = width;
    this->height = height;
}

void ProgramAdapter::setPosition(Vector3D position) {
    this->position = position;
}

void ProgramAdapter::addPosition(Vector3D offset) {
    this->position.x += offset.x;
    this->position.y += offset.y;
    this->position.z += offset.z;
}

void ProgramAdapter::setRotation(Vector3D position) {
    this->rotation = rotation;
}

void ProgramAdapter::addRotation(Vector3D offset) {
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;
}

void ProgramAdapter::setFOV(float fov) {
    this->fov = fov;
}

Vector3D ProgramAdapter::getForwardVector() {
    return calculateForwardVector(rotation);
}

Vector3D ProgramAdapter::getRightVector() {
    return calculateRightVector(rotation);
}