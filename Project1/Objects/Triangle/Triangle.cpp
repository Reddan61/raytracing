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

Triangle::TriangleShader Triangle::getShader()
{
    TriangleShader result;
    result.A = glm::vec4(this->A, 1.0f);
    result.B = glm::vec4(this->B, 1.0f);
    result.C = glm::vec4(this->C, 1.0f);
    result.color = glm::vec4(this->getColor(), 1.0f);
    result.reflective = this->getReflective();
    result.specular = this->getSpecular();
    result.single_side = this->isSingleSide;

    return result;
}

VkDeviceSize Triangle::getBufferSize()
{
    return sizeof(Triangle::TriangleShader);
}

void Triangle::changePosition(const glm::vec3 const& position)
{
    this->A = this->A + position;
    this->B = this->B + position;
    this->C = this->C + position;
}

void Triangle::update(GLFWwindow* window, float delta)
{
}
