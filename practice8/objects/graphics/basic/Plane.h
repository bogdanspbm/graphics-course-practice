//
// Created by Bogdan Madzhuga on 21.10.2023.
//

#ifndef PRACTICE6_PLANE_H
#define PRACTICE6_PLANE_H


#include "objects/graphics/Placeable.h"

class Plane : Placeable {

protected:
    void setProgram(ProgramAdapter *program) override;

public:
    Plane(ProgramAdapter *programAdapter);

    void draw() override;

    void setPosition(Vector3F position) override;

    void setRotation(Vector3F rotation) override;

    void setScale(Vector3F scale) override;

};


#endif //PRACTICE6_PLANE_H
