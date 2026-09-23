#pragma once

#include "common.h"
#include "Game/Components/DeadTag.h"
#include "ECS/EntityManager.h"

class CleanUpSystem
{
public:
    void Update(EntityManager &entityManager)
    {
        std::vector<uint32_t> deadEntities;

        uint32_t deadTagId = ComponentRegistry::GetId<DeadTag>();

        uint32_t deadTagSize = ComponentRegistry::globalSizeTable[deadTagId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(deadTagId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(chunk->data);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                deadEntities.push_back(indexEntityMap[i]);
            }
        }

        for (uint32_t entity : deadEntities)
        {
            entityManager.DestroyEntity(entity);
        }
    }
};