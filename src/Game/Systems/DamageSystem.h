#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Projectile.h"
#include "Game/Components/Hostile.h"
#include "Game/Components/HealthPoint.h"
#include "Game/Components/DeadTag.h"

class DamageSystem
{
    void ResolveDamage(EntityManager &entityManager, uint32_t hostileId, uint32_t projectileId);

public:
    void Update(EntityManager &entityManager);
};