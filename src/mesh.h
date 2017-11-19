#pragma once

#include <vector>
#include "../lib/tinyobjloader/tiny_obj_loader.h"

struct Mesh {
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
tinyobj::attrib_t attrib;

Mesh (std::string filePath);
};
