#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(
    std::vector<Triangle*>* triangles
)
{
    std::sort(triangles->begin(), triangles->end(), [](Triangle* a, Triangle* b) {
        return (a->getCentroid().y < b->getCentroid().y);
    });

    this->polygons = triangles;

    this->bvh = this->calculateBVH(*this->polygons, 5, 0, triangles->size());
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
    result.origin.last_index_node = bvhs_nodes_index;
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
    shader.last_index_node = -1;
    shader.self_index = bvh_node_index;
    shader.parent_node = parent_node;
    shader.box = node->box;

    if (node->left_node == nullptr && node->right_node == nullptr) {
        shader.left_node = -1;
        shader.right_node = -1;
        shader.triangles_start = node->start + triangles_offset;
        shader.triangles_end = node->end + triangles_offset;
    }
    else {
        size_t self = bvh_node_index;
        bvh_node_index += 1;
        shader.left_node = bvh_node_index;
        this->getBVHBufferRecursive(inside, node->left_node, bvh_node_index, triangles_offset, self);
        bvh_node_index += 1;
        shader.right_node = bvh_node_index;
        this->getBVHBufferRecursive(inside, node->right_node, bvh_node_index, triangles_offset, self);
    }

    result.push_back(shader);
    result.insert(result.end(), inside.begin(), inside.end());
}

TriangleMesh::BVHNode* TriangleMesh::calculateBVH(std::vector<Triangle*>& triangles, int depth, int start, int end)
{
    TriangleMesh::BVHNode* node = new TriangleMesh::BVHNode;

    std::vector<Triangle*> triangles_splitted;

    triangles_splitted.insert(
        triangles_splitted.end(),
        triangles.begin() + start,
        triangles.begin() + end
    );

    Triangle::AABB aabb = triangles_splitted[0]->getAABB();

    for (size_t i = 1; i < triangles_splitted.size(); i++) {
        Triangle::AABB local_aabb = triangles_splitted[i]->getAABB();

        aabb.min = glm::min(aabb.min, local_aabb.min);
        aabb.max = glm::max(aabb.max, local_aabb.max);
    }

    node->box = aabb;

    //if (triangles_splitted.size() == 1 || depth <= 0) {
    if (triangles_splitted.size() == 1) {
        node->left_node = nullptr;
        node->right_node = nullptr;
        node->start = start;
        node->end = end;
    }
    else {
        auto splitted = this->splitTriangles(start, end);

        node->left_node = this->calculateBVH(
            triangles,
            depth - 1, 
            splitted.triangles_left_start, 
            splitted.triangles_left_end
        );
        node->right_node = this->calculateBVH(
            triangles,
            depth - 1,
            splitted.triangles_right_start,
            splitted.triangles_right_end
        );
    }

    return node;
}

TriangleMesh::Split TriangleMesh::splitTriangles(
    int start, int end
)
{
    TriangleMesh::Split result;
    int center = (start + end) / 2;
    result.triangles_left_start = start;
    result.triangles_left_end = center;
    result.triangles_right_start = center;
    result.triangles_right_end = end;

    return result;
}
