#pragma once
#include "common.h"

class Camera2D
{
public:
    vec2 wCenter;
    vec2 wSize;
    Camera2D() : wCenter(0, 0), wSize(64, 64) {}

    mat4 V() { return translate(mat4(1), vec3(-wCenter.x, -wCenter.y, 0)); }
    mat4 P() { return scale(mat4(1), vec3(2.0f / wSize.x, 2.0f / wSize.y, 1.0f)); }

    mat4 Vinv() { return translate(mat4(1), vec3(wCenter.x, wCenter.y, 0)); }
    mat4 Pinv() { return scale(mat4(1), vec3(wSize.x / 2.0f, wSize.y / 2.0f, 1.0f)); }
};