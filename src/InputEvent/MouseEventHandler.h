#pragma once

enum MouseButton
{
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT
};

class MouseEventHandler
{
public:
    virtual void onMousePressed(MouseButton but, int pX, int pY) {}
    virtual void onMouseReleased(MouseButton but, int pX, int pY) {}
    virtual void onMouseMotion(int pX, int pY) {}
};