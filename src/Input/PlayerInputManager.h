#pragma once

#include <bitset>

class PlayerInputManager
{
    std::bitset<8> currentMouse;
    std::bitset<8> previousMouse;

public:
    double mouseX = 0.0;
    double mouseY = 0.0;
    double previousMouseX = 0.0;
    double previousMouseY = 0.0;

    void Update()
    {
        previousMouseX = mouseX;
        previousMouseY = mouseY;
        previousMouse = currentMouse;
    }

    void PressMouseButton(unsigned int keyCode, double x, double y)
    {
        if (keyCode < 8)
        {
            currentMouse.set(keyCode);
            mouseX = x;
            mouseY = y;
        }
    }

    void ReleaseMouseButton(unsigned int keyCode, double x, double y)
    {
        if (keyCode < 8)
        {
            currentMouse.reset(keyCode);
            mouseX = x;
            mouseY = y;
        }
        currentMouse.reset(keyCode);
    }

    bool IsMouseButtonHeld(unsigned int keyCode)
    {
        return currentMouse.test(keyCode);
    }

    bool IsMouseButtonJustPressed(unsigned int keyCode)
    {
        return !previousMouse.test(keyCode) && currentMouse.test(keyCode);
    }

    bool IsMouseButtonJustReleased(unsigned int keyCode)
    {
        return previousMouse.test(keyCode) && !currentMouse.test(keyCode);
    }
};