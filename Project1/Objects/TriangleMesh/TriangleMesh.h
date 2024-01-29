#pragma once
#include "../Object.h"
#include "../Triangle/Triangle.h"
#include "../../Math.h"

class TriangleMesh : public Object {
public:
	TriangleMesh(
		uint32_t numFaces, 
		const std::unique_ptr<uint32_t[]> const &faceIndex,
		const std::unique_ptr<uint32_t[]> const &vertsIndex,
		const std::unique_ptr<Vector3d[]> const &verts,
		const std::unique_ptr<Vector3d[]> const &normals,
		const std::unique_ptr<sf::Vector2f[]> const &st
	);
	~TriangleMesh();

	Vector3d getNormal(const Vector3d const& point, const Vector3d const& direction) override;

	void update(sf::RenderWindow& window, sf::Time time) override;
	Object::InsertRayValue insertRay(Vector3d& cameraPosition, Vector3d& direction) override;

	void changePosition(const Vector3d const& position) override;
private:
	std::vector<Triangle*> *polygons = nullptr;

	void render(sf::RenderWindow& window) override;
};

