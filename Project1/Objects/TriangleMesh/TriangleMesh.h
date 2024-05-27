#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../Object.h"
#include "../Triangle/Triangle.h"

class TriangleMesh : public Object {
public:
	struct BVHShader {
		int parent_node;
		int left_node, right_node;
		int right_sibling;
		int triangle_index;
		alignas(16) Triangle::AABB box;
	};

	struct BVHBuffer {
		TriangleMesh::BVHShader origin;
		std::vector<TriangleMesh::BVHShader> bvhs;
	};

	TriangleMesh(
		std::vector<Triangle*>* triangles
	);
	~TriangleMesh();

	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;

	std::vector<Triangle*>* getTriangles();
	size_t getTrianglesNum();

	void update(float delta) override;

	void changePosition(const glm::vec3 const& position) override;
	void rotate(float xAngle, float yAngle);

	BVHBuffer getBVHBuffer(size_t bvhs_offset, size_t triangles_offset);
private:
	struct Split {
		int triangles_left_start, triangles_left_end;
		int triangles_right_start, triangles_right_end;
		int center;
	};

	enum class BVH_Axis {
		X, Y, Z
	};

	struct BVHNode {
		size_t index;
		BVHNode* left_node;
		BVHNode* right_node;
		Triangle::AABB box;
	};

	std::vector<Triangle*> *polygons = nullptr;
	BVHNode* bvh = nullptr;

	TriangleMesh::BVHNode* calculateBVH(std::vector<Triangle*>* triangles, size_t start, size_t end);

	void sortTriangles(std::vector<Triangle*>* triangles, BVH_Axis axis);

	TriangleMesh::Split splitTriangles(
		size_t start, size_t end
	);
	void getBVHBufferRecursive(
		std::vector<TriangleMesh::BVHShader> &result, 
		BVHNode* node, 
		size_t& bvh_node_index,
		size_t triangles_offset,
		size_t parent_node
	);
};

