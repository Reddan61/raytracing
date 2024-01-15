#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "./Objects/TriangleMesh/TriangleMesh.h"

namespace Utils {
	TriangleMesh* LoadCustomFormatFile(const std::string const &filename);
}