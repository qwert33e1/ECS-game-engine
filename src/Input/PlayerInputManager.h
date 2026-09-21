#pragma once

#include <bitset>

class PlayerInputManager
{
    std::bitset<8> currentMouse;
    std::bitset<8> previousMouse;

    std::bitset<256> currentKeys;
    std::bitset<256> previousKeys;

public:
    float mouseX = 0.0;
    float mouseY = 0.0;
    float previousMouseX = 0.0;
    float previousMouseY = 0.0;

    void Update()
    {
        previousMouseX = mouseX;
        previousMouseY = mouseY;
        previousMouse = currentMouse;
        previousKeys = currentKeys;
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
    }

    void PressKey(unsigned int keyCode)
    {
        if (keyCode < 256)
        {
            currentKeys.set(keyCode);
        }
    }

    void ReleaseKey(unsigned int keyCode)
    {
        if (keyCode < 256)
        {
            currentKeys.reset(keyCode);
        }
    }

    void SetMousePosition(double x, double y)
    {
        mouseX = x;
        mouseY = y;
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

    bool IsKeyHeld(unsigned int keyCode)
    {
        return currentKeys.test(keyCode);
    }

    bool IsKeyJustPressed(unsigned int keyCode)
    {
        return !previousKeys.test(keyCode) && currentKeys.test(keyCode);
    }

    bool IsKeyJustReleased(unsigned int keyCode)
    {
        return previousKeys.test(keyCode) && !currentKeys.test(keyCode);
    }
};