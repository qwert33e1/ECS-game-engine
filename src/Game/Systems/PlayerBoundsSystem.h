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
        uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();
        uint32_t pcSize = ComponentRegistry::globalSizeTable[pcId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(pcId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(chunk->data);
            uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);

                float max_x = bounds->width / 2.0f;
                float min_x = -max_x;

                float max_y = bounds->height / 2.0f;
                float min_y = -max_y;

                pos->x = std::clamp(pos->x, min_x, max_x);
                pos->y = std::clamp(pos->y, min_y, max_y);
            }
        }
    }
};