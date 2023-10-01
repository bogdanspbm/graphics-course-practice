#pragma once

#include <vector>
#include <filesystem>
#include "objects/graphics/Model.h"


void fillModelFromFile(Model *model, std::filesystem::path const &path);
Model parseModel(std::filesystem::path const &path);
