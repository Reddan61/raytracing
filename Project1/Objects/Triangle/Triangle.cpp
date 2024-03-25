#include "Triangle.h"

Triangle::Triangle(
    const glm::vec3 const& A,
    const glm::vec3 const& B,
    const glm::vec3 const& C,
    const glm::vec3 const& color,
    float specular,
    float reflective
)
{
    this->A = A;
    this->B = B;
    this->C = C;

    this->color = color;
    this->specular = specular;
    this->reflective = reflective;
    this->isSingleSide = false;
}

Triangle::~Triangle()
{
}

glm::vec3 Triangle::getNormal(const glm::vec3 const& point, const glm::vec3 const& direction)
{
    glm::vec3 AB = this->B - this->A;
    glm::vec3 AC = this->C - this->A;

    glm::vec3 normal = glm::normalize(glm::cross(AB, AC));

    return normal;
}

//Object::InsertRayValue Triangle::insertRay(Vector3d& cameraPosition, Vector3d& direction)
//{
//    Vector3d e1 = this->B - this->A;
//    Vector3d e2 = this->C - this->A;
//
//    Vector3d pvec = Math::GetCrossProduct(direction, e2);
//    float det = Math::GetDotProduct(e1, pvec);
//
//
//    if (this->isSingleSide && det < Epsilon) {
//        return Object::InsertRayValue(Infinity, Infinity, nullptr);
//    }
//    else if (det < Epsilon && det > -Epsilon) {
//        return Object::InsertRayValue(Infinity, Infinity, nullptr);
//    }
//
//    float inv_det = 1.0f / det;
//
//    Vector3d tvec = cameraPosition - this->A;
//    float v = inv_det * Math::GetDotProduct(tvec, pvec);
//
//    if (v < 0.0f || v > 1.0f) {
//        return Object::InsertRayValue(Infinity, Infinity, nullptr);
//    }
//
//    Vector3d qvec = Math::GetCrossProduct(tvec, e1);
//    float w = inv_det * Math::GetDotProduct(direction, qvec);
//
//    if (w < 0.0f || w + v > 1.0f) {
//        return Object::InsertRayValue(Infinity, Infinity, nullptr);
//    }
//
//    float t = inv_det * Math::GetDotProduct(e2, qvec);
//
//    if (t > Epsilon) return Object::InsertRayValue(t, t, this);
//
//    return Object::InsertRayValue(Infinity, Infinity, nullptr);
//}

void Triangle::changePosition(const glm::vec3 const& position)
{
    this->A = this->A + position;
    this->B = this->B + position;
    this->C = this->C + position;
}

void Triangle::update(GLFWwindow* window, float delta)
{
}
