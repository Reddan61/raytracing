#pragma once
#include <vector>
#include <memory>
#include "../Object.h"
#include "../Triangle/Triangle.h"

class TriangleMesh : public Object {
public:
	TriangleMesh(
		std::vector<Triangle*>* triangles
	);
	~TriangleMesh();

	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;


	std::vector<Triangle*>* getPolygons();
	size_t getPolygonsSize();
	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
private:
	std::vector<Triangle*> *polygons = nullptr;
};

