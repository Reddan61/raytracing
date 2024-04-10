#include "Sky.h"

glm::vec3 Sky::getColor(const glm::vec3&direction)
{
    glm::vec4 base(133, 249, 255, 255);
    glm::vec3 normalized = glm::normalize(direction);

    float pos = 0.5 * (normalized.y + 1.0);

    return (pos * base) + ((1 - pos) * glm::vec4(255, 255, 255, 255));
}
