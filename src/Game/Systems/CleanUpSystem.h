#pragma once

#include "common.h"
#include "Game/Components/DeadTag.h"
#include "ECS/EntityManager.h"

class CleanUpSystem
{
public:
    void Update(EntityManager &entityManager);
};