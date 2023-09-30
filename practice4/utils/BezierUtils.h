//
// Created by Bogdan Madzhuga on 24.09.2023.
//

#ifndef PRACTICE3_BEZIERUTILS_H
#define PRACTICE3_BEZIERUTILS_H


#include "structures/Vertex.h"
#include "structures/Vector.h"
#include <vector>

std::vector<Vertex> computeBezierCurve(const std::vector<Vertex> &points, int quality);

Vertex calculateBezierPoint(const std::vector<Vertex> &points, float t);

std::vector<Vector> computeBezierColors(const std::vector<Vertex> &points);


#endif //PRACTICE3_BEZIERUTILS_H
