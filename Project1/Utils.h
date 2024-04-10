#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "./Objects/TriangleMesh/TriangleMesh.h"

namespace Utils {
	TriangleMesh* LoadCustomFormatFile(const std::string const &filename);
    std::vector<Triangle*>* loadOBJ(const std::string& filename);
}