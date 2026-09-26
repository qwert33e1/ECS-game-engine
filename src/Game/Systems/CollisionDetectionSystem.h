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
public:
    void Update(EntityManager &entityManager)
    {
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

        SpatialGrid grid(100.0f, bounds->width, bounds->height);

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

        auto cells = grid.GetCells();

        for (auto const &cell : cells)
        {
            for (size_t i = 0; i < cell.size(); i++)
            {
                for (size_t j = i + 1; j < cell.size(); j++)
                {
                    float dx = cell[i].x - cell[j].x;
                    float dy = cell[i].y - cell[j].y;
                    float distSq = (dx * dx) + (dy * dy);

                    float radiusSum = cell[i].radius + cell[j].radius;

                    if (distSq < (radiusSum * radiusSum))
                    {
                        printf("e1: %d, e2: %d\n", cell[i].id, cell[j].id);
                        entityManager.PushCollisionEvent(CollisionEvent{cell[i].id, cell[j].id});
                    }
                }
            }
        }
    }
};