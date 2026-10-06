#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Speed.h"
#include "Game/Components/Projectile.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Sprite.h"
#include "Game/Components/Rotation.h"
#include "Input/PlayerInputManager.h"
#include "Utility/Coordinates.h"
#include <glm/glm.hpp>

class PlayerInputSystem
{
public:
    void Update(EntityManager &entityManager, PlayerInputManager &inputManager);
};