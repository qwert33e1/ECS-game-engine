#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Speed.h"
#include "Input/PlayerInputManager.h"
#include <glm/glm.hpp>

class PlayerInputSystem
{
public:
    void Update(EntityManager &entityManager, PlayerInputManager &inputManager)
    {
        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();
        uint32_t targetId = ComponentRegistry::GetId<TargetPosition>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t pcSize = ComponentRegistry::globalSizeTable[pcId];
        uint32_t targetSize = ComponentRegistry::globalSizeTable[targetId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(pcId, true);
        bitmask.set(targetId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t posOffset = chunk->archetype->getComponentOffset(posId);
            uint32_t targetOffset = chunk->archetype->getComponentOffset(targetId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                TargetPosition *target = reinterpret_cast<TargetPosition *>(chunk->data + targetOffset + i * targetSize);

                if (inputManager.IsMouseButtonJustPressed(1))
                {
                    target->x = inputManager.mouseX;
                    target->y = inputManager.mouseY;

                    printf("SET TARGET :: target : (%f, %f) - pos : (%f, %f)\n ", target->x, target->y, pos->x, pos->y);
                }

                if (inputManager.IsKeyJustPressed('Q'))
                {
                    float dist = glm::length(glm::vec2(inputManager.mouseX - pos->x, inputManager.mouseY - pos->y));

                    entityManager.CreateEntity(Position{pos->x, pos->y}, Velocity{(inputManager.mouseX - pos->x) / dist, (inputManager.mouseY - pos->y) / dist}, Sprite{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)});
                }
            }
        }
    }
};