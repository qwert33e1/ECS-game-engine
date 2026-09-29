#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Hostile.h"
#include "Game/Components/Speed.h"
#include "Game/Components/Collider.h"
#include "Input/PlayerInputManager.h"
#include "Utility/Coordinates.h"
#include <glm/glm.hpp>

class HostileFollowSystem
{
public:
    void Update(EntityManager &entityManager);
};