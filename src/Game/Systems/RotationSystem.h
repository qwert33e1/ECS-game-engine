#pragma once

#include <cmath>
#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Rotation.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/Projectile.h"

class RotationSystem
{
public:
    void Update(EntityManager &entityManager);
};