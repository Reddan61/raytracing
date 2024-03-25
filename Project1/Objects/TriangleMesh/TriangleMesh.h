#pragma once
#include <vector>
#include "../Object.h"
#include "../Triangle/Triangle.h"

class TriangleMesh : public Object {
public:
	TriangleMesh(
		//uint32_t numFaces, 
		//const std::unique_ptr<uint32_t[]> const &faceIndex,
		//const std::unique_ptr<uint32_t[]> const &vertsIndex,
		//const std::unique_ptr<Vector3d[]> const &verts,
		//const std::unique_ptr<Vector3d[]> const &normals,
		//const std::unique_ptr<sf::Vector2f[]> const &st
	);
	~TriangleMesh();

	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;

	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
private:
	std::vector<Triangle*> *polygons = nullptr;
};

