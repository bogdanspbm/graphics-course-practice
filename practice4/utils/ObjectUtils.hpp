#pragma once

#include <vector>
#include <filesystem>
#include "objects/Model.h"


Model parseModel(std::filesystem::path const & path);
