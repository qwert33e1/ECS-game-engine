#pragma once
#include <functional>

/* obsolete */
enum MouseButton
{
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT
};

class MouseEventHandler
{
public:
    std::function<void(MouseButton but, int pX, int pY)> onMousePressed;
    std::function<void(MouseButton but, int pX, int pY)> onMouseReleased;
};