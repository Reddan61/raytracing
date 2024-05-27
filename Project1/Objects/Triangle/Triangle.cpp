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
    this->A = glm::vec4(A, 1.0f);
    this->B = glm::vec4(B, 1.0f);
    this->C = glm::vec4(C, 1.0f);

    this->color = color;
    this->specular = specular;
    this->reflective = reflective;
    this->isSingleSide = false;
    this->rotation_matrix = new RotationMatrix(0.0f, 0.0f, false);

    this->calculateCentroid();
}

Triangle::~Triangle()
{
    if (this->rotation_matrix != nullptr) {
        delete this->rotation_matrix;
    }
}

glm::vec3 Triangle::getNormal(const glm::vec3 const& point, const glm::vec3 const& direction)
{
    glm::vec3 AB = this->B - this->A;
    glm::vec3 AC = this->C - this->A;

    glm::vec3 normal = glm::normalize(glm::cross(AB, AC));

    return normal;
}

Triangle::TriangleShader Triangle::getShader()
{
    TriangleShader result;
    result.A = this->A;
    result.B = this->B;
    result.C = this->C;
    result.color = glm::vec4(this->getColor(), 1.0f);
    result.reflective = this->getReflective();
    result.specular = this->getSpecular();
    result.single_side = this->isSingleSide ? 1 : 0;

    return result;
}

void Triangle::rotate(float xAngle, float yAngle, glm::vec4 center)
{
    this->Object::rotate(xAngle, yAngle);

    //auto centroid = this->getCentroid();

    this->A -= center;
    this->B -= center;
    this->C -= center;

    this->A = this->Object::rotation_matrix->getRotation() * this->A;
    this->B = this->Object::rotation_matrix->getRotation() * this->B;
    this->C = this->Object::rotation_matrix->getRotation() * this->C;

    this->A += center;
    this->B += center;
    this->C += center;
}

VkDeviceSize Triangle::getBufferSize()
{
    return sizeof(Triangle::TriangleShader);
}

void Triangle::changePosition(const glm::vec3 const& position)
{
    this->A = this->A + glm::vec4(position, 1.0f);
    this->B = this->B + glm::vec4(position, 1.0f);
    this->C = this->C + glm::vec4(position, 1.0f);

    this->calculateCentroid();
}

glm::vec4 Triangle::getCentroid()
{
    return this->centroid;
}

void Triangle::calculateCentroid()
{
    this->centroid = (this->A + this->B + this->C) / 3.0f;
}

Triangle::AABB Triangle::getAABB()
{
    Triangle::AABB result;

    result.min = result.max = this->A;

    result.min = glm::min(result.min, this->B);
    result.min = glm::min(result.min, this->C);
    result.max = glm::max(result.max, this->B);
    result.max = glm::max(result.max, this->C);

    return result;
}

void Triangle::update(float delta)
{
}
