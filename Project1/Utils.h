#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "./Objects/TriangleMesh/TriangleMesh.h"

namespace Utils {
    std::vector<Triangle*>* loadOBJ(const std::string& filename, const glm::vec4& const color, float specular, float reflective);
}