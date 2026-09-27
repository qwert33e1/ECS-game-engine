#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Speed.h"
#include <glm/glm.hpp>

class TargetSystem
{
public:
    void Update(EntityManager &entityManager, float dt)
    {
        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t velId = ComponentRegistry::GetId<Velocity>();
        uint32_t speedId = ComponentRegistry::GetId<Speed>();
        uint32_t targetId = ComponentRegistry::GetId<TargetPosition>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t velSize = ComponentRegistry::globalSizeTable[velId];
        uint32_t speedSize = ComponentRegistry::globalSizeTable[speedId];
        uint32_t targetSize = ComponentRegistry::globalSizeTable[targetId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(velId, true);
        bitmask.set(speedId, true);
        bitmask.set(targetId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
            uint32_t velOffset = chunk->archetype->GetComponentOffset(velId);
            uint32_t speedOffset = chunk->archetype->GetComponentOffset(speedId);
            uint32_t targetOffset = chunk->archetype->GetComponentOffset(targetId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Velocity *vel = reinterpret_cast<Velocity *>(chunk->data + velOffset + i * velSize);
                Speed *speed = reinterpret_cast<Speed *>(chunk->data + speedOffset + i * speedSize);
                TargetPosition *target = reinterpret_cast<TargetPosition *>(chunk->data + targetOffset + i * targetSize);

                float dist = glm::length(glm::vec2(target->x - pos->x, target->y - pos->y));
                float stepDistance = dt * speed->current;

                // Stops when the target is reached
                if (dist < stepDistance)
                {
                    vel->x = 0.0;
                    vel->y = 0.0;
                    pos->x = target->x;
                    pos->y = target->y;
                }
                else
                {
                    vel->x = (target->x - pos->x) / dist * speed->current;
                    vel->y = (target->y - pos->y) / dist * speed->current;
                }
            }
        }
    }
};