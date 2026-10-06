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
#include "Game/Systems/HostileFollowSystem.h"

class Game
{
    EntityManager entityManager;
    Renderer2D &renderer;
    MovementSystem movementSystem;
    RenderSystem renderSystem = RenderSystem(renderer);
    PlayerInputSystem playerInputSystem;
    PlayerInputManager &inputManager;
    TargetSystem targetSystem;
    CameraFollowSystem cameraFollowSystem;
    OutOfBoundsSystem outOfBoundsSystem;
    CleanUpSystem cleanUpSystem;
    PlayerBoundsSystem playerBoundsSystem;
    CollisionDetectionSystem collisionDetectionSystem;
    DamageSystem damageSystem;
    HostileFollowSystem hostileFollowSystem;

    // should not be here
    float mapWidth = 10000.0f;
    float mapHeight = 10000.0f;
    float cellSize = 256.0f;
    SpatialGrid grid;

public:
    Game(Renderer2D &renderer, PlayerInputManager &inputManager) : renderer(renderer), inputManager(inputManager), grid(cellSize, mapWidth, mapHeight)
    {
        this->entityManager.CreateEntity(Camera{300.0f, 300.0f, 1.0f, WINDOW_WIDTH, WINDOW_HEIGHT});
        this->entityManager.CreateEntity(Position{0.0f, 0.0f}, Velocity{0.0f, 0.0f}, Sprite{"hehe.png", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, PlayerControlled{}, TargetPosition{0.0f, 0.0f}, Speed{100.0f}, Collider{25.0f});
        this->entityManager.CreateEntity(Position{200.0f, 200.0f}, Velocity{0.0f, 0.0f}, Sprite{"monster.png", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}, TargetPosition{0.0f, 0.0f}, Hostile{}, HealthPoint{10.0f, 10.0f}, Speed{60.0f}, Collider{25.0f});
        this->entityManager.CreateEntity(MapBounds{mapWidth, mapHeight});
        renderSystem.LoadTexture();
    }

    void UpdateVariable()
    {
        playerInputSystem.Update(entityManager, inputManager);
    }

    void UpdateFixed(float dt)
    {
        movementSystem.Update(entityManager, dt);
        hostileFollowSystem.Update(entityManager);
        targetSystem.Update(entityManager, dt);
        playerBoundsSystem.Update(entityManager);
        outOfBoundsSystem.Update(entityManager);
        collisionDetectionSystem.Update(entityManager, grid);
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
        renderSystem.Update(entityManager);
    }
};
