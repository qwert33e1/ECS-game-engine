#pragma once
#include "common.h"
#include "ECS/EntityManager.h"
#include "InputEvent/MouseEventHandler.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/Sprite.h"
#include "Renderer/Renderer2D.h"
#include "Game/Systems/MovementSystem.h"
#include "Game/Systems/RenderSystem.h"

class Game
{
    MouseEventHandler mouseEventHandler;
    EntityManager entityManager;
    Renderer2D &renderer;
    MovementSystem movementSystem = MovementSystem(entityManager);
    RenderSystem renderSystem = RenderSystem(entityManager, renderer);

public:
    Game(Renderer2D &renderer) : renderer(renderer)
    {
        mouseEventHandler.onMousePressed = [this](MouseButton but, int pX, int pY)
        {
            this->entityManager.CreateEntity(Position{(float)pX, (float)pY}, Velocity{0.0f, 0.0f}, Sprite{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)});
        };
        mouseEventHandler.onMouseReleased = [this](MouseButton but, int pX, int pY) {};
    }

    void Update()
    {
        movementSystem.Update(0.1f);
        renderSystem.Draw();
    }

    MouseEventHandler &GetMouseHandler()
    {
        return mouseEventHandler;
    }
};
