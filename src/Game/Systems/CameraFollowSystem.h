#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Position.h"
#include "Game/Components/PlayerControlled.h"

class CameraFollowSystem
{
public:
    void Update(EntityManager &entityManager);
};