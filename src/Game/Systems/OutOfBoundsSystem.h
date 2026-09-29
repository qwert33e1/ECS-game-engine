#pragma once

#include "common.h"
#include "Game/Components/Position.h"
#include "Game/Components/MapBounds.h"
#include "Game/Components/DeadTag.h"
#include "ECS/EntityManager.h"

class OutOfBoundsSystem
{
public:
    void Update(EntityManager &entityManager);
};