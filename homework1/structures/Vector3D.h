//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#ifndef HOMEWORK1_VECTOR3D_H
#define HOMEWORK1_VECTOR3D_H


class Vector3D {
public:
    float x = 0, y = 0, z = 0;

    Vector3D() {

    }

    Vector3D(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
};


#endif //HOMEWORK1_VECTOR3D_H
