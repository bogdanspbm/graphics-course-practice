//
// Created by Bogdan Madzhuga on 24.09.2023.
//

#include "BezierUtils.h"

std::vector<Vertex> computeBezierCurve(const std::vector<Vertex> &points, int quality) {
    std::vector<Vertex> bezierCurvePoints;

    for (int i = 0; i <= quality; ++i) {
        float t = (float) (i) / quality;
        Vertex pointOnCurve = calculateBezierPoint(points, t);
        bezierCurvePoints.push_back(pointOnCurve);
    }

    return bezierCurvePoints;
}

std::vector<Vector> computeBezierColors(const std::vector<Vertex> &points) {
    std::vector<Vector> bezierColors;

    for (int i = 0; i <= points.size(); ++i) {
        Vector color = Vector{(float) (rand() % 1000) / 1000, (float) (rand() % 1000) / 1000,
                              (float) (rand() % 1000) / 1000, 1.f};
        bezierColors.push_back(color);
    }

    return bezierColors;
}

Vertex calculateBezierPoint(const std::vector<Vertex> &points, float t) {
    int n = points.size() - 1;

    if (n == 0) {
        return points[0];
    }

    std::vector<Vertex> newControlPoints;
    for (int i = 0; i < n; ++i) {
        float x = (1 - t) * points[i].x + t * points[i + 1].x;
        float y = (1 - t) * points[i].y + t * points[i + 1].y;
        float z = (1 - t) * points[i].z + t * points[i + 1].z;
        newControlPoints.push_back({x, y, z});
    }

    return calculateBezierPoint(newControlPoints, t);
}