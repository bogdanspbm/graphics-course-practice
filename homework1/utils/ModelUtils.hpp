#pragma once

#include <vector>
#include <filesystem>
#include "objects/graphics/Renderable.h"


void fillModelFromFile(Renderable *model, std::filesystem::path const &path);
Renderable parseModel(std::filesystem::path const &path);
