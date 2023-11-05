//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "ProgramAdapter.h"
#include "utils/MathUtils.h"

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
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    int textureLocation = glGetUniformLocation(this->id, texture->getLayerName().c_str());

    if (textureLocation != -1) {
        glUniform1i(textureLocation, 0); // 0 corresponds to GL_TEXTURE0
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
