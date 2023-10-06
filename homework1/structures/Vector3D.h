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
        return Vector3D(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    Vector3D operator+(Vector3D vector) const {
        return Vector3D(this->x + vector.x, this->y + vector.y, this->z + vector.z);
    }

    Vector3D operator-(Vector3D vector) const {
        return Vector3D(this->x - vector.x, this->y - vector.y, this->z - vector.z);
    }
};


#endif //HOMEWORK1_VECTOR3D_H
