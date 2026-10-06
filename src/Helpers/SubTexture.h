#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include "Renderer/Texture.h"

class SubTexture
{
    uint32_t textureId;
    glm::vec2 uvMin;
    glm::vec2 uvMax;

public:
    SubTexture() = default;

    SubTexture(std::shared_ptr<Texture> texture, float x, float y, float width, float height)
    {
        textureId = texture->GetId();
        uvMin = {x / texture->GetWidth(),
                 y / texture->GetHeight()};

        uvMax = {(x + width) / texture->GetWidth(),
                 (y + height) / texture->GetHeight()};
    }

    uint32_t GetId() { return textureId; }
    glm::vec2 GetUVMin() { return uvMin; }
    glm::vec2 GetUVMax() { return uvMax; }
};