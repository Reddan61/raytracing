#include "Triangle.h"

Triangle::Triangle(const sf::Vector3f const& A, const sf::Vector3f const& B, const sf::Vector3f const& C, const sf::Color const& color, float specular, float reflective)
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

sf::Vector3f Triangle::getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction)
{
    sf::Vector3f AB = this->B - this->A;
    sf::Vector3f AC = this->C - this->A;

    sf::Vector3f normal = Math::normalize(Math::GetCrossProduct(AB, AC));

    return normal;
}

Object::InsertRayValue Triangle::insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction)
{
    sf::Vector3f e1 = this->B - this->A;
    sf::Vector3f e2 = this->C - this->A;

    sf::Vector3f pvec = Math::GetCrossProduct(direction, e2);
    float det = Math::GetDotProduct(e1, pvec);


    if (this->isSingleSide && det < Epsilon) {
        return Object::InsertRayValue(Infinity, Infinity, nullptr);
    }
    else if (det < Epsilon && det > -Epsilon) {
        return Object::InsertRayValue(Infinity, Infinity, nullptr);
    }

    float inv_det = 1.0f / det;

    sf::Vector3f tvec = cameraPosition - this->A;
    float v = inv_det * Math::GetDotProduct(tvec, pvec);

    if (v < 0.0f || v > 1.0f) {
        return Object::InsertRayValue(Infinity, Infinity, nullptr);
    }

    sf::Vector3f qvec = Math::GetCrossProduct(tvec, e1);
    float w = inv_det * Math::GetDotProduct(direction, qvec);

    if (w < 0.0f || w + v > 1.0f) {
        return Object::InsertRayValue(Infinity, Infinity, nullptr);
    }

    float t = inv_det * Math::GetDotProduct(e2, qvec);

    if (t > Epsilon) return Object::InsertRayValue(t, t, this);

    return Object::InsertRayValue(Infinity, Infinity, nullptr);
}

void Triangle::changePosition(const sf::Vector3f const& position)
{
    this->A = this->A + position;
    this->B = this->B + position;
    this->C = this->C + position;
}

void Triangle::update(sf::RenderWindow& window, sf::Time time)
{
}

void Triangle::render(sf::RenderWindow& window)
{
}
