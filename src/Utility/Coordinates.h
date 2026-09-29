#pragma once
#include "Game/Components/Camera.h"
#include <glm/glm.hpp>

namespace Coordinates
{
    inline glm::vec2 WorldToScreen(float wX, float wY, Camera camera)
    {
        float sX = (wX - camera.x) + (camera.screenWidth / 2.0f);
        float sY = (wY - camera.y) + (camera.screenHeight / 2.0f);

        return glm::vec2(sX, sY);
    }

    inline glm::vec2 ScreenToWorld(float sX, float sY, Camera camera)
    {
        float wX = camera.x + (sX - camera.screenWidth / 2.0f);
        float wY = camera.y + (sY - camera.screenHeight / 2.0f);

        return glm::vec2(wX, wY);
    }
}