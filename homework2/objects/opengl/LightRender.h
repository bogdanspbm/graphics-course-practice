//
// Created by Bogdan Madzhuga on 10.11.2023.
//

#ifndef HOMEWORK2_LIGHTRENDER_H
#define HOMEWORK2_LIGHTRENDER_H


#include <vector>
#include "objects/graphics/light/structures/DirectionLight.h"
#include "objects/graphics/light/Sun.h"
#include "objects/graphics/renderable/Renderable.h"
#include "GLProgram.h"
#include "objects/graphics/renderable/Placeable.h"

class LightRender {
private:
    static LightRender *instance;
    std::vector<DirectionLight> lights;
    int curSource = 0;

public:

    static LightRender *getLightRenderer() {
        if (instance != nullptr) {
            return instance;
        }

        instance = new LightRender();
        return instance;
    }

public:
    void addDirectionLight(DirectionLight light) {
        lights.push_back(light);
        this->nextSource();
    }

    glm::vec3 getLightColor() {
        if (curSource == lights.size()) {
            return Sun::getSun()->getColor();
        }

        auto light = lights[curSource];
        return light.color;
    }

    glm::vec3 getLightDirection() {
        if (curSource == lights.size()) {
            return Sun::getSun()->getDirection();
        }

        auto light = lights[curSource];
        return light.direction;
    }

    float getLightRange() {
        if (curSource == lights.size()) {
            return 0;
        }

        auto light = lights[curSource];
        return light.range;
    }

    float getLightFov() {
        if (curSource == lights.size()) {
            return 90;
        }

        auto light = lights[curSource];
        return light.fov;
    }

    float getLightFar() {
        if (curSource == lights.size()) {
            return Sun::getSun()->getHeight();
        }

        auto light = lights[curSource];
        return light.range;
    }


    glm::vec3 getRotation() {
        if (curSource == lights.size()) {
            return Sun::getSun()->getRotation();
        }

        auto light = lights[curSource];
        glm::vec3 rotation = directionToRotation(light.direction);
        return rotation;
    }

    glm::vec3 getLocation() {
        if (curSource == lights.size()) {
            return Sun::getSun()->getLocation();
        }

        auto light = lights[curSource];
        return light.location;
    }

    void nextSource() {
        curSource = (curSource + 1) % (lights.size() + 1);
    }

    void renderLight(std::vector<Placeable *> list)
    {
        curSource = lights.size();
        for (int i = 0; i <= lights.size(); i++) {
            GLProgram::getGLProgram(SHADOW)->useProgram();

            for (int i = 0; i < list.size(); i++) {
                list[i]->draw();
            }

            GLProgram::getGLProgram(LIGHT)->useProgram();

            for (int i = 0; i < list.size(); i++) {
                list[i]->draw();
            }

            GLProgram::getGLProgram()->setUniformFloat("lightRange", getLightRange());
            GLProgram::getGLProgram()->setUniformVector3F("lightPosition", getLocation());
            GLProgram::getGLProgram()->setUniformVector3F("inputSunColor", getLightColor());
            GLProgram::getGLProgram()->setUniformVector3F("inputSunDirection", glm::normalize(getLightDirection()));

            nextSource();
            return;
        }
    }

};


#endif //HOMEWORK2_LIGHTRENDER_H
