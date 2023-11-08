//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "ProgramAdapter.h"
#include "utils/MathUtils.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

void ProgramAdapter::useProgram(SDL_Window *window) {
    glUseProgram(this->id);
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    if (frameBuffer != nullptr) {
        frameBuffer->bindFrameBuffer();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    setProjectionMatrix();
    setViewMatrix();
    setLight();
    bindPointLights();
    bindShadowMap();
    setUniformVector3F("view_direction", getForwardVector());
    setUniformVector3F("view_position", this->position);
    setUniformFloat("near", near);
    setUniformFloat("far", far);
}

void ProgramAdapter::setUniformMatrix4FV(const GLchar *name, GLfloat *value, bool transpose) {
    GLint uniformLocationID = glGetUniformLocation(id, name);

    if (uniformLocationID == -1) {
        // throw std::invalid_argument(TAG + ": Can't find uniform location: " + name);
    } else {
        glUniformMatrix4fv(uniformLocationID, 1, transpose ? GL_TRUE : GL_FALSE, value);
    }
}

void ProgramAdapter::setUniformVector3F(const GLchar *name, Vector3F vector) {
    GLint uniformLocationID = glGetUniformLocation(id, name);

    if (uniformLocationID == -1) {
        // throw std::invalid_argument(TAG + ": Can't find uniform location: " + name);
    } else {
        glUniform3f(uniformLocationID, vector.x, vector.y, vector.z);
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

    if (texture->isBindable()) {

        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

        int textureLocation = glGetUniformLocation(this->id, texture->getLayerName().c_str());

        if (textureLocation != -1) {
            glUniform1i(textureLocation, 0); // 0 corresponds to GL_TEXTURE0
        }
    }

    setUniformVector3F("albedo", texture->getAlbedo());
    setUniformFloat("roughness", texture->getRoughness());
    setUniformFloat("glossiness", texture->getGlossiness());
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
    setUniformMatrix4FV("view", view, true);
}

void ProgramAdapter::setLight() {
    setUniformVector3F("ambient_light", ambientLight);
    setUniformVector3F("sun_color", sunLightColor);
    setUniformVector3F("sun_direction", sunDirection);

    if (this->lightAdapter != nullptr) {
        float sunView[16];
        this->lightAdapter->calcViewMatrix(sunView);
        setUniformMatrix4FV("sun_view", sunView, true);
    }
}

void ProgramAdapter::calcViewMatrix(float *matrix) {
    memset(matrix, 0, sizeof(float) * 16);

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                               * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                               * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Create a translation matrix based on the position vector
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));

    // Calculate the view matrix by combining the rotation and translation matrices
    glm::mat4 viewMatrix = translationMatrix * rotationMatrix;

    // Transpose the view matrix to obtain the column-major format
    glm::mat4 transposedViewMatrix = glm::transpose(viewMatrix);

    // Copy the transposed view matrix to the output array
    memcpy(matrix, &transposedViewMatrix[0][0], sizeof(float) * 16);
}

void ProgramAdapter::setResolution(float width, float height) {
    this->width = width;
    this->height = height;
}

void ProgramAdapter::setPosition(Vector3F position) {
    this->position = position;
}

void ProgramAdapter::addPosition(Vector3F offset) {
    this->position.x += offset.x;
    this->position.y += offset.y;
    this->position.z += offset.z;
}

void ProgramAdapter::setRotation(Vector3F rotation) {
    this->rotation = rotation;
}

void ProgramAdapter::addRotation(Vector3F rotation) {
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;
}

void ProgramAdapter::setFOV(float fov) {
    this->fov = fov;
}

Vector3F ProgramAdapter::getForwardVector() {
    return calculateForwardVector(rotation);
}

Vector3F ProgramAdapter::getRightVector() {
    return calculateRightVector(rotation);
}

void ProgramAdapter::setFrameBuffer(FrameBuffer *buffer) {
    frameBuffer = buffer;
}

void ProgramAdapter::setFrameBuffer(int width, int height) {
    frameBuffer = new FrameBuffer(width, height);
}

Texture *ProgramAdapter::getTexture() {
    if (frameBuffer == nullptr) {
        return nullptr;
    }

    return frameBuffer->getTexture();
}

void ProgramAdapter::setClearColor(Vector4F clearColor) {
    if (frameBuffer == nullptr) {
        return;
    }

    frameBuffer->setClearColor(clearColor);
}

void ProgramAdapter::setAmbientLight(Vector3F light) {
    this->ambientLight = light;
}

void ProgramAdapter::setSunLight(Vector3F color, Vector3F direction) {
    this->sunLightColor = color;
    this->sunDirection = direction;
}

void ProgramAdapter::setUniformFloat(const GLchar *name, float value) {
    GLint uniformLocationID = glGetUniformLocation(id, name);

    if (uniformLocationID == -1) {
        // throw std::invalid_argument(TAG + ": Can't find uniform location: " + name);
    } else {
        glUniform1f(uniformLocationID, value);
    }
}

void ProgramAdapter::addPointLight(PointLight point) {
    lightPoints.push_back(point);
}

void ProgramAdapter::bindPointLights() {
    for (int i = 0; i < lightPoints.size(); i++) {
        PointLight light = lightPoints[i];
        std::string itemName = "pointLights[" + std::to_string(i) + "]";
        setUniformVector3F((itemName + ".position").c_str(), light.position);
        setUniformVector3F((itemName + ".color").c_str(), light.color);
        setUniformVector3F((itemName + ".attenuation").c_str(), light.attenuation);
    }
}


void ProgramAdapter::setLightAdapter(ProgramAdapter *adapter) {
    this->lightAdapter = adapter;
}

void ProgramAdapter::setShadowMap(Texture *shadowMap) {
    this->shadowMap = shadowMap;
}

void ProgramAdapter::bindShadowMap() {
    if (shadowMap == nullptr) {
        return;
    }
    glBindTexture(GL_TEXTURE_2D, shadowMap->getTextureID());

    int textureLocation = glGetUniformLocation(this->id, "shadow_map");

    if (textureLocation != -1) {
        glUniform1i(textureLocation, 0); // 0 corresponds to GL_TEXTURE0
    }

}
