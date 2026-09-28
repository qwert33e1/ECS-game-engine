#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Collider.h"
#include "Game/Components/MapBounds.h"
#include "DataStructures/SpatialGrid.h"
#include "DataStructures/CollisionEvent.h"
#include <glm/glm.hpp>

class CollisionDetectionSystem
{
    void CheckCellAgainstCell(EntityManager &entityManager, std::vector<FatCell> const &cell1, std::vector<FatCell> const &cell2, bool sameCell)
    {
        for (auto const &entity1 : cell1)
        {
            for (auto const &entity2 : cell2)
            {
                if (sameCell)
                {
                    if (entity1.id < entity2.id)
                    {
                        float dx = entity1.x - entity2.x;
                        float dy = entity1.y - entity2.y;
                        float distSq = (dx * dx) + (dy * dy);

                        float radiusSum = entity1.radius + entity2.radius;

                        if (distSq < (radiusSum * radiusSum))
                        {
                            entityManager.PushCollisionEvent(CollisionEvent{entity1.id, entity2.id});
                        }
                    }
                }
            }
        }
    }

public:
    void Update(EntityManager &entityManager, SpatialGrid &grid)
    {
        grid.Clear();

        // getting the MapBounds entity
        uint32_t boundsId = ComponentRegistry::GetId<MapBounds>();
        uint32_t boundsSize = ComponentRegistry::globalSizeTable[boundsId];
        std::bitset<MAX_COMPONENTS> boundsMask;
        boundsMask.set(boundsId, true);
        auto boundsChunk = (entityManager.GetEntities(boundsMask)).front();
        if (boundsChunk->entityCounter == 0)
        {
            return;
        }
        uint32_t boundsOffset = boundsChunk->archetype->GetComponentOffset(boundsId);
        MapBounds *bounds = reinterpret_cast<MapBounds *>(boundsChunk->data + boundsOffset);

        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];

        uint32_t colliderId = ComponentRegistry::GetId<Collider>();
        uint32_t colliderSize = ComponentRegistry::globalSizeTable[colliderId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(colliderId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(chunk->data);
            uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
            uint32_t colliderOffset = chunk->archetype->GetComponentOffset(colliderId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Collider *collider = reinterpret_cast<Collider *>(chunk->data + colliderOffset + i * colliderSize);

                grid.Add(pos->x, pos->y, collider->radius, indexEntityMap[i]);
            }
        }

        auto &cells = grid.GetCells();
        auto &activeCells = grid.GetActiveCells();

        for (auto const &cell : activeCells)
        {
            int gridWidth = grid.getGridWidth();
            int gridHeight = grid.getGridHeight();

            int gridX = cell.x;
            int gridY = cell.y;
            auto &currentCell = cells[gridY * gridWidth + gridX];

            CheckCellAgainstCell(entityManager, currentCell, currentCell, true);

            if (gridX + 1 < gridWidth)
                CheckCellAgainstCell(entityManager, currentCell, cells[gridY * gridWidth + (gridX + 1)], false);
            if (gridX + 1 < gridWidth && gridY + 1 < gridHeight)
                CheckCellAgainstCell(entityManager, currentCell, cells[(gridY + 1) * gridWidth + gridX], false);
            if (gridY + 1 < gridHeight)
                CheckCellAgainstCell(entityManager, currentCell, cells[(gridY + 1) * gridWidth + gridX], false);
            if (gridX - 1 >= 0 && gridY + 1 < gridHeight)
                CheckCellAgainstCell(entityManager, currentCell, cells[(gridY + 1) * gridWidth + (gridX - 1)], false);
        }
    }
};