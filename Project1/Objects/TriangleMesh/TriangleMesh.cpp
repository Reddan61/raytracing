#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(
    uint32_t numFaces,
    // кол-во вершин на грани
    const std::unique_ptr<uint32_t[]> const& faceIndex,
    const std::unique_ptr<uint32_t[]> const& vertsIndex,
    // вершины
    const std::unique_ptr<glm::vec3[]> const& verts
)
{
    this->specular = 0.2f;
    this->reflective = 0.0f;

    int offset = 0;
    std::vector<glm::vec3> polygonVert;
    polygonVert.reserve(3);

    auto *result = new std::vector<Triangle*>;

    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < faceIndex[i]; j++) {
            int vertId = vertsIndex[offset + j];
            glm::vec3 vert = verts[vertId];

             polygonVert.push_back(vert);

             if (polygonVert.size() == 3) {
                 result->push_back(new Triangle(
                     polygonVert[0],
                     polygonVert[1],
                     polygonVert[2],
                     glm::vec3(1.0f, 0, 0),
                     this->specular,
                     this->reflective
                 ));

                 polygonVert.erase(polygonVert.begin() + 1);
             }
        }

        offset += faceIndex[i];
        polygonVert.clear();
    }

    this->polygons = result;
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
