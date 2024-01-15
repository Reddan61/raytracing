#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(
    uint32_t numFaces,
    // кол-во вершин на грани
    const std::unique_ptr<uint32_t[]> const& faceIndex,
    const std::unique_ptr<uint32_t[]> const& vertsIndex,
    // вершины
    const std::unique_ptr<sf::Vector3f[]> const& verts,
    const std::unique_ptr<sf::Vector3f[]> const& normals,
    const std::unique_ptr<sf::Vector2f[]> const& st)
{
    this->specular = 1000;
    this->reflective = 0.5f;

    int offset = 0;
    std::vector<sf::Vector3f> polygonVert;
    polygonVert.reserve(3);

    auto *result = new std::vector<Triangle*>;

    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < faceIndex[i]; j++) {
            int vertId = vertsIndex[offset + j];
            sf::Vector3f vert = verts[vertId];

             polygonVert.push_back(vert);

             if (polygonVert.size() == 3) {
                 result->push_back(new Triangle(
                     polygonVert[0],
                     polygonVert[1],
                     polygonVert[2],
                     sf::Color(255, 0, 0, 255),
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

sf::Vector3f TriangleMesh::getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction)
{
    return sf::Vector3f();
}

void TriangleMesh::update(sf::RenderWindow& window, sf::Time time)
{
}

Object::InsertRayValue TriangleMesh::insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction)
{
    if (this->polygons == nullptr) return Object::InsertRayValue(Infinity, Infinity, nullptr);

    int size = this->polygons->size();
    Object::InsertRayValue result(Infinity, Infinity, nullptr);

    for (int i = 0; i < size; i++) {
        Triangle *triangle = this->polygons->at(i);

        Object::InsertRayValue t = triangle->insertRay(cameraPosition, direction);

        if (t.t1 < result.t1) {
            result.t1 = t.t1;
            result.t2 = t.t1;
            result.object = triangle;
        } 
        if (t.t2 < result.t2) {
            result.t1 = t.t2;
            result.t2 = t.t2;
            result.object = triangle;
        }
    }

    return result;
}

void TriangleMesh::changePosition(const sf::Vector3f const& position)
{
    if (this->polygons == nullptr) return;

    int size = this->polygons->size();

    for (int i = 0; i < size; i++) {
        Triangle* triangle = this->polygons->at(i);
        
        triangle->changePosition(position);
    }
}

void TriangleMesh::render(sf::RenderWindow& window)
{
}
