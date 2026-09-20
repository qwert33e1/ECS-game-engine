#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Input/PlayerInputManager.h"
#include <glm/glm.hpp>

class PlayerInputSystem
{
public:
    void Update(EntityManager &entityManager, PlayerInputManager &inputManager)
    {
        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t velId = ComponentRegistry::GetId<Velocity>();
        uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();
        uint32_t targetId = ComponentRegistry::GetId<TargetPosition>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t velSize = ComponentRegistry::globalSizeTable[velId];
        uint32_t pcSize = ComponentRegistry::globalSizeTable[pcId];
        uint32_t targetSize = ComponentRegistry::globalSizeTable[targetId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(velId, true);
        bitmask.set(pcId, true);
        bitmask.set(targetId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t posOffset = chunk->archetype->getComponentOffset(posId);
            uint32_t velOffset = chunk->archetype->getComponentOffset(velId);
            uint32_t targetOffset = chunk->archetype->getComponentOffset(targetId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Velocity *vel = reinterpret_cast<Velocity *>(chunk->data + velOffset + i * velSize);
                TargetPosition *target = reinterpret_cast<TargetPosition *>(chunk->data + targetOffset + i * targetSize);

                if (inputManager.IsMouseButtonJustPressed(1))
                {
                    float dist = glm::length(glm::vec2(inputManager.mouseX - pos->x, inputManager.mouseY - pos->y));

                    vel->x = (inputManager.mouseX - pos->x) / dist;
                    vel->y = (inputManager.mouseY - pos->y) / dist;

                    target->x = inputManager.mouseX;
                    target->y = inputManager.mouseY;

                    printf("SET TARGET :: target : (%f, %f) - pos : (%f, %f) - vel : (%f, %f) - dist: %f\n ", target->x, target->y, pos->x, pos->y, vel->x, vel->y, dist);
                }
            }
        }
    }
};