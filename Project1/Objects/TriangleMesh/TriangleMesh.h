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
		const std::unique_ptr<sf::Vector3f[]> const &verts,
		const std::unique_ptr<sf::Vector3f[]> const &normals,
		const std::unique_ptr<sf::Vector2f[]> const &st
	);
	~TriangleMesh();

	sf::Vector3f getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction) override;

	void update(sf::RenderWindow& window, sf::Time time) override;
	Object::InsertRayValue insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction) override;

	void changePosition(const sf::Vector3f const& position) override;
private:
	std::vector<Triangle*> *polygons = nullptr;

	void render(sf::RenderWindow& window) override;
};

