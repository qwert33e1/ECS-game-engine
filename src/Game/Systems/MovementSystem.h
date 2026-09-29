#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"

class MovementSystem
{
public:
    void Update(EntityManager &entityManager, float dt);
};