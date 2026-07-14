#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"

class MovementSystem
{
    EntityManager &entityManager;

public:
    MovementSystem(EntityManager &em) : entityManager(em) {}

    void Update(double dt)
    {
        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t velId = ComponentRegistry::GetId<Velocity>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t velSize = ComponentRegistry::globalSizeTable[velId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(velId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t maxEntityInChunk = chunk->archetype->getMaxEntityInChunk();
            uint32_t posOffset = chunk->archetype->getComponentOffset(posId);
            uint32_t velOffset = chunk->archetype->getComponentOffset(velId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Velocity *vel = reinterpret_cast<Velocity *>(chunk->data + velOffset + i * velSize);

                pos->x += dt * vel->x;
                pos->y += dt * vel->y;
            }
        }
    }
};