#pragma once

#include <algorithm>
#include "common.h"
#include "Game/Components/Position.h"
#include "Game/Components/MapBounds.h"
#include "Game/Components/PlayerControlled.h"
#include "ECS/EntityManager.h"

class PlayerBoundsSystem
{
public:
    void Update(EntityManager &entityManager);
};