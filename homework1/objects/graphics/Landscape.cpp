//
// Created by Bogdan Madzhuga on 01.10.2023.
//

#include "Landscape.h"

Landscape::Landscape(std::function<float(float, float)> function) {
    this->heightFunction = function;
}

void Landscape::draw() {
    Model::draw();
}
