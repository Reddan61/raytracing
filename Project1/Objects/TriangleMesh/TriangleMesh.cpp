#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(
    std::vector<Triangle*>* triangles
)
{
    this->sortTriangles(triangles, BVH_Axis::Y);

    this->polygons = triangles;
    this->rotation_matrix = new RotationMatrix(0.0f, 0.0f);

    this->bvh = this->calculateBVH(this->polygons, 0, this->polygons->size());
}

TriangleMesh::~TriangleMesh()
{
    if (this->polygons != nullptr) {
        delete polygons;
    }

    if (this->rotation_matrix != nullptr) {
        delete this->rotation_matrix;
    }
}

glm::vec3 TriangleMesh::getNormal(const glm::vec3 const& point, const glm::vec3 const& direction)
{
    return glm::vec3();
}

std::vector<Triangle*>* TriangleMesh::getTriangles()
{
    return this->polygons;
}

size_t TriangleMesh::getTrianglesNum()
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

    this->bvh = this->calculateBVH(this->polygons, 0, this->polygons->size());
}

void TriangleMesh::rotate(float xAngle, float yAngle)
{
    if (this->polygons == nullptr) return;

    int size = this->polygons->size();

    glm::vec4 center = glm::vec4(0.0f);

    for (int i = 0; i < size; i++) {
        Triangle* triangle = this->polygons->at(i);

        center += triangle->getCentroid();
    }

    center /= size;

    for (int i = 0; i < size; i++) {
        Triangle* triangle = this->polygons->at(i);

        triangle->rotate(xAngle, yAngle, center);
    }

    this->bvh = this->calculateBVH(this->polygons, 0, this->polygons->size());
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
    shader.parent_node = parent_node;
    shader.box = node->box;

    if (node->left_node == nullptr && node->right_node == nullptr) {
        //shader.triangle_index = node->triangle.self_index + triangles_offset;
        shader.triangle_index = node->index + triangles_offset;
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

TriangleMesh::BVHNode* TriangleMesh::calculateBVH(std::vector<Triangle*>* triangles, size_t start, size_t end)
{
    TriangleMesh::BVHNode* node = new TriangleMesh::BVHNode;

    std::vector<Triangle*> triangles_splitted;

    triangles_splitted.insert(
        triangles_splitted.end(),
        triangles->begin() + start,
        triangles->begin() + end
    );

    Triangle::AABB aabb = triangles_splitted[0]->getAABB();

    for (size_t i = 1; i < triangles_splitted.size(); i++) {
        Triangle::AABB local_aabb = triangles_splitted[i]->getAABB();

        aabb.min = glm::min(aabb.min, local_aabb.min);
        aabb.max = glm::max(aabb.max, local_aabb.max);
    }

    node->box = aabb;

    if (triangles_splitted.size() == 1) {
        node->left_node = nullptr;
        node->right_node = nullptr;
        node->index = start;
    }
    else {
        auto splitted = this->splitTriangles(start, end);

        node->left_node = this->calculateBVH(
            triangles,
            splitted.triangles_left_start,
            splitted.triangles_left_end
        );
        node->right_node = this->calculateBVH(
            triangles,
            splitted.triangles_right_start,
            splitted.triangles_right_end
        );
    }

    return node;
}

void TriangleMesh::sortTriangles(std::vector<Triangle*>* triangles, BVH_Axis axis)
{
    int transormed_axis = static_cast<int>(axis);

    std::sort(triangles->begin(), triangles->end(), [transormed_axis](Triangle* t1, Triangle* t2) {
        return t1->getCentroid()[transormed_axis] < t2->getCentroid()[transormed_axis];
    });
}

TriangleMesh::Split TriangleMesh::splitTriangles(
    size_t start, size_t end
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
