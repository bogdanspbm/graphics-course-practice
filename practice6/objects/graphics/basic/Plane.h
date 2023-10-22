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

    void setPosition(Vector3D position) override;

    void setRotation(Vector3D rotation) override;

    void setScale(Vector3D scale) override;

};


#endif //PRACTICE6_PLANE_H
