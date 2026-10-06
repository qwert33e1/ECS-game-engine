#pragma once
#include <glm/glm.hpp>

namespace Transformation
{
    inline glm::vec2 rotatePoint(glm::vec2 p, glm::vec2 pos, float rotation)
    {
        float c = std::cos(rotation);
        float s = std::sin(rotation);

        return glm::vec2(pos.x + (p.x * c - p.y * s), pos.y + (p.x * s + p.y * c));
    }
}