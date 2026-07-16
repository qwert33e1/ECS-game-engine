#pragma once
#include "common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:
    glm::vec2 wCenter;
    glm::vec2 wSize;
    Camera2D() : wCenter(0, 0), wSize(WINDOW_WIDTH, WINDOW_HEIGHT) {}

    glm::mat4 V() { return glm::translate(glm::mat4(1), glm::vec3(-wCenter.x, -wCenter.y, 0)); }
    glm::mat4 P() { return glm::scale(glm::mat4(1), glm::vec3(2.0f / wSize.x, 2.0f / wSize.y, 1.0f)); }

    glm::mat4 Vinv() { return glm::translate(glm::mat4(1), glm::vec3(wCenter.x, wCenter.y, 0)); }
    glm::mat4 Pinv() { return glm::scale(glm::mat4(1), glm::vec3(wSize.x / 2.0f, wSize.y / 2.0f, 1.0f)); }
};