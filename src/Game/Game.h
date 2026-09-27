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
#include "Game/Components/Hostile.h"
#include "Renderer/Renderer2D.h"
#include "Game/Systems/MovementSystem.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/PlayerInputSystem.h"
#include "Game/Systems/TargetSystem.h"
#include "Game/Systems/CameraFollowSystem.h"
#include "Game/Systems/OutOfBoundsSystem.h"
#include "Game/Systems/CleanUpSystem.h"
#include "Game/Systems/PlayerBoundsSystem.h"
#include "Game/Systems/CollisionDetectionSystem.h"
#include "Game/Systems/DamageSystem.h"

class Game
{
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
    PlayerBoundsSystem playerBoundsSystem;
    CollisionDetectionSystem collisionDetectionSystem;
    DamageSystem damageSystem;

public:
    Game(Renderer2D &renderer, PlayerInputManager &inputManager) : renderer(renderer), inputManager(inputManager)
    {
        this->entityManager.CreateEntity(Camera{300.0f, 300.0f, 1.0f, WINDOW_WIDTH, WINDOW_HEIGHT});
        this->entityManager.CreateEntity(Position{300.0f, 300.0f}, Velocity{0.0f, 0.0f}, Sprite{glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, PlayerControlled{}, TargetPosition{300.0f, 300.0f}, Speed{100.0f}, Collider{25.0f});
        // this->entityManager.CreateEntity(Position{200.0f, 200.0f}, Velocity{0.0f, 0.0f}, Sprite{glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, Hostile{}, HealthPoint{10.0f, 10.0f}, Speed{5.0f}, Collider{25.0f});
        this->entityManager.CreateEntity(MapBounds{10000.0f, 10000.0f});
    }

    void UpdateVariable()
    {
        playerInputSystem.Update(entityManager, inputManager);
    }

    void UpdateFixed(float dt)
    {
        movementSystem.Update(dt);
        targetSystem.Update(entityManager, dt);
        playerBoundsSystem.Update(entityManager);
        outOfBoundsSystem.Update(entityManager);
        collisionDetectionSystem.Update(entityManager);
        damageSystem.Update(entityManager);
        cleanUpSystem.Update(entityManager);
        entityManager.ClearCollisionEvents();
    }

    void LateUpdate()
    {
        cameraFollowSystem.Update(entityManager);
    }

    void Render()
    {
        renderSystem.Draw();
    }
};
