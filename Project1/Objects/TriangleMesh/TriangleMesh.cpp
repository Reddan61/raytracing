#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(
    std::vector<Triangle*>* triangles
)
{
    this->polygons = triangles;

    std::vector<BVH_Triangle> bvh_triangles;
    bvh_triangles.reserve(triangles->size());

    for (size_t i = 0; i < triangles->size(); i++) {
        BVH_Triangle bvh_triangle;
        bvh_triangle.self_index = i;
        bvh_triangle.triangle = triangles->at(i);

        bvh_triangles.push_back(bvh_triangle);
    }

    this->bvh = this->calculateBVH(bvh_triangles, 5);
}

TriangleMesh::~TriangleMesh()
{
    if (this->polygons != nullptr) {
        delete polygons;
    }
}

glm::vec3 TriangleMesh::getNormal(const glm::vec3 const& point, const glm::vec3 const& direction)
{
    return glm::vec3();
}

std::vector<Triangle*>* TriangleMesh::getPolygons()
{
    return this->polygons;
}

size_t TriangleMesh::getPolygonsSize()
{
    return this->polygons->size();
}

void TriangleMesh::update(GLFWwindow* window, float delta)
{
}

void TriangleMesh::changePosition(const glm::vec3 const& position)
{
    if (this->polygons == nullptr) return;

    int size = this->polygons->size();

    for (int i = 0; i < size; i++) {
        Triangle* triangle = this->polygons->at(i);
        
        triangle->changePosition(position);
    }
}

TriangleMesh::BVHBuffer TriangleMesh::getBVHBuffer(size_t bvhs_offset, size_t triangles_offset)
{
    TriangleMesh::BVHBuffer result;
    size_t bvhs_nodes_index = bvhs_offset - 1;
    this->getBVHBufferRecursive(result.bvhs, this->bvh, bvhs_nodes_index, triangles_offset, -1);

    result.origin = result.bvhs[0];
    result.bvhs.erase(result.bvhs.begin());

    return result;
}

void TriangleMesh::getBVHBufferRecursive(
    std::vector<TriangleMesh::BVHShader>& result, 
    BVHNode* node,
    size_t& bvh_node_index,
    size_t triangles_offset,
    size_t parent_node
)
{
    TriangleMesh::BVHShader shader;
    std::vector<TriangleMesh::BVHShader> inside;
    shader.right_sibling = -1;
    shader.left_node = -1;
    shader.right_node = -1;
    //shader.self_index = bvh_node_index;
    shader.parent_node = parent_node;
    shader.box = node->box;

    if (node->left_node == nullptr && node->right_node == nullptr) {
        shader.triangle_index = node->triangle.self_index + triangles_offset;
        /*
        shader.triangles_start = node->start + triangles_offset;
        shader.triangles_end = node->end + triangles_offset;*/
    }
    else {
        size_t self = bvh_node_index;
        bvh_node_index += 1;
        shader.left_node = bvh_node_index;
        this->getBVHBufferRecursive(inside, node->left_node, bvh_node_index, triangles_offset, self);
        bvh_node_index += 1;
        shader.right_node = bvh_node_index;
        this->getBVHBufferRecursive(inside, node->right_node, bvh_node_index, triangles_offset, self);
        inside[0].right_sibling = shader.right_node;
    }

    result.push_back(shader);
    result.insert(result.end(), inside.begin(), inside.end());
}

TriangleMesh::BVH_SAH TriangleMesh::sortTriangles(std::vector<BVH_Triangle>& triangles)
{
    TriangleMesh::BVH_SAH result;
    result.best_axis = BVH_Axis::X;
    result.best_pos = 0.0f;
    auto best_cost = INFINITY;

    for (auto axis : { BVH_Axis::X, BVH_Axis::Y, BVH_Axis::Z }) {
        for (size_t i = 0; i < triangles.size(); i++) {
            Triangle* triangle = triangles.at(i).triangle;

            float pos = triangle->getCentroid()[static_cast<int>(axis)];
            float cost = this->SAHCost(triangles, axis, pos);

            if (cost < best_cost) {
                best_cost = cost;
                result.best_axis = axis;
                result.best_pos = pos;
                result.triangle = triangle;
            }
        }
    }

    return result;
}

float TriangleMesh::SAHCost(std::vector<BVH_Triangle>& triangles, BVH_Axis axis, float pos)
{
    Triangle::AABB left_box, right_box;
    size_t left_count = 0, right_count = 0;

    for (size_t i = 0; i < triangles.size(); i++) {
        Triangle* triangle = triangles.at(i).triangle;

        auto centroid = triangle->getCentroid()[static_cast<int>(axis)];

        if (centroid < pos) {
            left_count++;
            left_box.grow(triangle->getAABB());
        }
        else {
            right_count++;
            right_box.grow(triangle->getAABB());
        }
    }

    if (left_count < 1 || right_count < 1) {
        return INFINITY;
    }

    float cost = left_count * left_box.area() + right_count * right_box.area();

    return cost > 0 ? cost : INFINITY;
}

TriangleMesh::BVHNode* TriangleMesh::calculateBVH(std::vector<BVH_Triangle>& triangles, int depth)
{
    if (triangles.empty()) {
        return nullptr;
    }

    TriangleMesh::BVHNode* node = new TriangleMesh::BVHNode;

    auto sorted = this->sortTriangles(triangles);

    Triangle::AABB aabb = triangles[0].triangle->getAABB();

    for (size_t i = 1; i < triangles.size(); i++) {
        Triangle::AABB local_aabb = triangles[i].triangle->getAABB();

        aabb.min = glm::min(aabb.min, local_aabb.min);
        aabb.max = glm::max(aabb.max, local_aabb.max);
    }

    node->box = aabb;

    if (triangles.size() == 1) {
        node->left_node = nullptr;
        node->right_node = nullptr;
        node->triangle = triangles.at(0);
    }
    else {
        std::vector<TriangleMesh::BVH_Triangle> triangles_left, triangles_right;

        this->splitTriangles(triangles, sorted, triangles_left, triangles_right);

        node->left_node = this->calculateBVH(
            triangles_left,
            0
        );
        node->right_node = this->calculateBVH(
            triangles_right,
            0
        );
    }

    return node;
}

void TriangleMesh::splitTriangles(
    std::vector<TriangleMesh::BVH_Triangle>& triangles,
    BVH_SAH sah,
    std::vector<TriangleMesh::BVH_Triangle>& triangles_left,
    std::vector<TriangleMesh::BVH_Triangle>& triangles_right
)
{
    int axis = static_cast<int>(sah.best_axis);

    for (size_t i = 0; i < triangles.size(); i++) {
        Triangle* triangle = triangles.at(i).triangle;

        float pos = triangle->getCentroid()[axis];

        if (pos < sah.best_pos) {
            triangles_left.push_back(triangles.at(i));
        }
        else {
            triangles_right.push_back(triangles.at(i));
        }
    }
}
