#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../Object.h"
#include "../Triangle/Triangle.h"

class TriangleMesh : public Object {
public:
	struct BVHNode {
		int start, end;
		BVHNode* left_node;
		BVHNode* right_node;
		Triangle::AABB box;
	};

	struct BVHShader {
		int left_node, right_node;
		int triangles_start, triangles_end;
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

	std::vector<Triangle*>* getPolygons();
	size_t getPolygonsSize();
	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
	BVHBuffer getBVHBuffer(size_t bvhs_offset, size_t triangles_offset);
private:
	std::vector<Triangle*> *polygons = nullptr;
	BVHNode* bvh = nullptr;

	TriangleMesh::BVHNode* calculateBVH(std::vector<Triangle*>& triangles, int depth, int start, int end);

	struct Split {
		int triangles_left_start, triangles_left_end;
		int triangles_right_start, triangles_right_end;
		int center;
	};

	Split splitTriangles(
		int start, int end
	);
	void getBVHBufferRecursive(
		std::vector<TriangleMesh::BVHShader> &result, 
		BVHNode* node, 
		size_t& bvh_node_index,
		size_t triangles_offset
	);
};

