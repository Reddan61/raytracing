#include "Sky.h"

Color Sky::getColor(const Vector3d &direction)
{
    Color base(133, 249, 255, 255);
    Vector3d normalized = direction.normalize();

    float pos = 0.5 * (normalized.y + 1.0);

    return (pos * base) + ((1 - pos) * Color(255, 255, 255, 255));
}
