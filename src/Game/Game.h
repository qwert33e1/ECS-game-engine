#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Input/MouseEventHandler.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/Sprite.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Speed.h"
#include "Game/Components/Camera.h"
#include "Game/Components/MapBounds.h"
#include "Renderer/Renderer2D.h"
#include "Game/Systems/MovementSystem.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/PlayerInputSystem.h"
#include "Game/Systems/TargetSystem.h"
#include "Game/Systems/CameraFollowSystem.h"
#include "Game/Systems/OutOfBoundsSystem.h"
#include "Game/Systems/CleanUpSystem.h"

class Game
{
    MouseEventHandler mouseEventHandler;
    EntityManager entityManager;
    Renderer2D &renderer;
    MovementSystem movementSystem = MovementSystem(entityManager);
    RenderSystem renderSystem = RenderSystem(entityManager, renderer);
    PlayerInputSystem playerInputSystem;
    PlayerInputManager &inputManager;
    TargetSystem targetSystem;
    CameraFollowSystem cameraFollowSystem;
    OutOfBoundsSystem outOfBoundsSystem;
    CleanUpSystem cleanUpSystem;

public:
    Game(Renderer2D &renderer, PlayerInputManager &inputManager) : renderer(renderer), inputManager(inputManager)
    {
        this->entityManager.CreateEntity(Camera{300.0f, 300.0f, 1.0f, 600, 600});
        this->entityManager.CreateEntity(Position{300.0f, 300.0f}, Velocity{0.0f, 0.0f}, Sprite{glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, PlayerControlled{}, TargetPosition{300.0f, 300.0f}, Speed{5.0f});
        this->entityManager.CreateEntity(MapBounds{5000.0f, 5000.0f});
    }

    void Update()
    {
        movementSystem.Update(0.1f);
        renderSystem.Draw();
        targetSystem.Update(entityManager);
        cameraFollowSystem.Update(entityManager);
        playerInputSystem.Update(entityManager, inputManager);
        outOfBoundsSystem.Update(entityManager);
        cleanUpSystem.Update(entityManager);
    }

    MouseEventHandler &GetMouseHandler()
    {
        return mouseEventHandler;
    }
};
