#pragma once

#include "common.h"
#include "Game/Components/Position.h"
#include "Game/Components/MapBounds.h"
#include "Game/Components/DeadTag.h"
#include "ECS/EntityManager.h"

class OutOfBoundsSystem
{
public:
    void Update(EntityManager &entityManager)
    {
        std::vector<uint32_t> outOfBoundsEntities;

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

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(chunk->data);
            uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);

                if (abs(pos->x) > (bounds->width / 2.0f) || abs(pos->y) > (bounds->height / 2.0f))
                {
                    outOfBoundsEntities.push_back(indexEntityMap[i]);
                }
            }
        }

        for (uint32_t entity : outOfBoundsEntities)
        {
            entityManager.AddComponent<DeadTag>(entity, DeadTag{});
            printf("DEADTAG\n");
        }
    }
};