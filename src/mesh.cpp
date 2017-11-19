#define TINYOBJLOADER_IMPLEMENTATION

#include <string>
#include <iostream>
#include "mesh.h"

Mesh::Mesh(std::string filePath) {
    // Load Mesh from obj
    std::string err;
    std::cout << "Loading mesh: " + filePath << '\n';
    tinyobj::LoadObj(&attrib, &shapes, &materials,
        &err, filePath.c_str());

    // If error loading obj, write message to stderr
    if (!err.empty()) {
      std::cerr << err << std::endl;
    }
}
