//
// Created by Bogdan Madzhuga on 02.10.2023.
//

#ifndef HOMEWORK1_VECTOR3D_H
#define HOMEWORK1_VECTOR3D_H


class Vector3F {
public:
    float x = 0, y = 0, z = 0;

    Vector3F() {

    }

    Vector3F(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3F operator*(float scalar) const {
        return Vector3F(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    Vector3F operator+(Vector3F vector) const {
        return Vector3F(this->x + vector.x, this->y + vector.y, this->z + vector.z);
    }

    Vector3F operator-(Vector3F vector) const {
        return Vector3F(this->x - vector.x, this->y - vector.y, this->z - vector.z);
    }
};


#endif //HOMEWORK1_VECTOR3D_H
