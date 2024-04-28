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
		//int parent_node, self_index;
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

	std::vector<Triangle*>* getPolygons();
	size_t getPolygonsSize();
	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
	BVHBuffer getBVHBuffer(size_t bvhs_offset, size_t triangles_offset);
private:
	struct BVH_Triangle {
		int self_index;
		Triangle* triangle;
	};

	struct Split {
		int triangles_left_start, triangles_left_end;
		int triangles_right_start, triangles_right_end;
		int center;
	};

	enum class BVH_Axis {
		X, Y, Z
	};

	struct BVH_SAH {
		BVH_Axis best_axis;
		float best_pos;
		Triangle* triangle;
	};

	struct BVHNode {
		TriangleMesh::BVH_Triangle triangle;
		BVHNode* left_node;
		BVHNode* right_node;
		Triangle::AABB box;
	};

	std::vector<Triangle*> *polygons = nullptr;
	BVHNode* bvh = nullptr;

	TriangleMesh::BVHNode* calculateBVH(std::vector<TriangleMesh::BVH_Triangle>& triangles, int depth);

	void splitTriangles(
		std::vector<TriangleMesh::BVH_Triangle>& triangles,
		BVH_SAH sah,
		std::vector<TriangleMesh::BVH_Triangle>& triangles_left,
		std::vector<TriangleMesh::BVH_Triangle>& triangles_right
	);
	void getBVHBufferRecursive(
		std::vector<TriangleMesh::BVHShader> &result, 
		BVHNode* node, 
		size_t& bvh_node_index,
		size_t triangles_offset,
		size_t parent_node
	);

	TriangleMesh::BVH_SAH sortTriangles(std::vector<BVH_Triangle>& triangles);
	float SAHCost(std::vector<BVH_Triangle>& triangles, BVH_Axis axis, float pos);
};

