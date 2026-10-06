#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Collider.h"
#include "Game/Components/MapBounds.h"
#include "Helpers/SpatialGrid.h"
#include "Helpers/CollisionEvent.h"
#include <glm/glm.hpp>

class CollisionDetectionSystem
{
    void CheckCellAgainstCell(EntityManager &entityManager, std::vector<FatCell> const &cell1, std::vector<FatCell> const &cell2, bool sameCell);

public:
    void Update(EntityManager &entityManager, SpatialGrid &grid);
};