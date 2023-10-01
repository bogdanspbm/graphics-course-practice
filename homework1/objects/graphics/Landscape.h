//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#ifndef PRACTICE4_LANDSCAPE_H
#define PRACTICE4_LANDSCAPE_H


#include "Model.h"

class Landscape : Model {
private:
    std::function<float(float x, float y)> heightFunction;
public:
    Landscape(std::function<float(float x, float y)> function);
    void draw();
};


#endif //PRACTICE4_LANDSCAPE_H
